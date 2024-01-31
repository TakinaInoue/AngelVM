#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "AngelArch.h"
#include "AngelMemory.h"
#include "AngelBytecode.h"

#ifdef AngelVM_Debug
#include "AngelDissasembler.h"
#endif

typedef struct CallFrame {
    uint8_t* ip;
    Fragment* fragment;
    ValueArray locals;
} Callframe;

#define Core_CallStackSize 512
#define Core_StackSize 256

typedef struct AngelCore {
    CallFrame callStack[512];
    CallFrame* callFrameTop;
    Value* stack[256];
    Value** stackTop;
} AngelCore;

void CoreReset(AngelCore*);

AngelCore* CreateAngelCore() {
    AngelCore* cc = (AngelCore*) malloc(sizeof(struct AngelCore));
    size_t i;
    for (i = 0; i < Core_CallStackSize; i++) {
        cc->callStack[i].ip = NULL;
        cc->callStack[i].fragment = NULL;
        InitValueArray(&cc->callStack[i].locals);
    }
    CoreReset(cc);
    return cc;
}

void CoreReset(AngelCore* core) {
    for (CallFrame* ptr = core->callFrameTop; ptr > core->callStack; ptr++) {
        for (size_t i = 0; i < ptr->locals.length; i++) {
         //   FreeValue(ptr->locals.data[i]);
        }
        free(&ptr->locals.data);
        InitValueArray(&ptr->locals);
        ptr->fragment = NULL;
        ptr->ip = NULL;
    }
    core->callFrameTop = core->callStack;
    core->stackTop = core->stack;
}

void CoreAddFrame(AngelCore* core,Fragment* fragment) {
    core->callFrameTop->ip = fragment->instructions.data;
    core->callFrameTop->fragment = fragment;
}

void CorePush_(AngelCore* core, Value* v) {
    *core->stackTop = v;
    core->stackTop++;
}

Value* CorePop_(AngelCore* core) {
    core->stackTop--;
    return *core->stackTop;
}

uint8_t CoreReadByte(AngelCore* core) {
    return *core->callFrameTop->ip++;
}

uint16_t CoreReadShort(AngelCore* core) {
    uint8_t a, b;
    // gona have to end like this
    a = CoreReadByte(core);
    b = CoreReadByte(core);
    return Join(a, b);
}

void CoreExecute(AngelCore* core) {
    for (;;) {
#ifdef AngelVM_Debug
    for (Value** ptr = core->stack; ptr < core->stackTop; ptr++) {
        printf("[%s]", ValueAsString(*ptr));
    }
    printf("\n");
    DissasembleInst(core->callFrameTop->fragment, (size_t)(core->callFrameTop->ip - core->callFrameTop->fragment->instructions.data));
#endif
#define ArithInst(opname, operat) case OpSet::opname: { \
                Value* a = Pop(); \
                Value* b = Pop(); \
                a = MakeCompatible(a, a->type < b->type ? a->type : b->type); \
                b = MakeCompatible(b, a->type < b->type ? a->type : b->type);  \
                if (a->type == ValueFloat) \
                    a->as.fp32 = a->as.fp32 operat b->as.fp32; \
                else if (a->type == ValueDouble) \
                    a->as.fp64 = a->as.fp64 operat b->as.fp64; \
                else  \
                    a->as.int64 = a->as.int64 operat b->as.int64; \
                Push(a); \
                break; \
            }


#define Push(v) CorePush_(core, v)
#define Pop() CorePop_(core)

#define ReadByte() CoreReadByte(core)
#define ReadShort() CoreReadShort(core)

    uint8_t it = ReadByte();

    switch(it) {
        case OpSet::Push: {
            uint16_t constIndex = ReadShort();
            printf("%zu\n", constIndex);
            Push(core->callFrameTop->fragment->values.data[constIndex]);
            break;
        }
        case OpSet::Pop: {
            Pop();
            break;
        }
        case OpSet::Return: {
            printf(" == vm quit == \n");
            return;
        }
        ArithInst(Add, +)
        ArithInst(Sub, -)
        ArithInst(Mul, *)
        ArithInst(Div, /)
        case OpSet::Mod: {
            Value* a = Pop(); 
            Value* b = Pop(); 
            if (a->type <= ValueDouble)
                a = MakeCompatible(a, ValueInt64);
            if (b->type <= ValueDouble)
                b = MakeCompatible(b, ValueInt64);
            a->as.int64 = a->as.int64 % b->as.int64;
            Push(a);
            break;
        }
        default:
            printf("//// CRITICAL ERROR ////\n");
            printf("COULD NOT RECOGNIZE OPCODE %u\n", it);
            printf("Cannot continue execution of Fragment.\n");
            printf("(please report this to https://takina.jp.net/projects/AngelVM/issues)\n");
            printf("Send this information in the header:\n");
            printf("AngelVM Architecture: %u", AngelVM_Arch);
            printf("Build Date: %s", __DATE__);
            printf("quitting..\n");
            exit(51851);
            break;
    }

#undef ReadByte
#undef ReadShort

#undef Push
#undef Pop

#undef ArithInst
    }
}