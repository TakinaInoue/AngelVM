#include "ARC.h"
#include "Bytecode.h"
#include "Dissasembler.h"

#ifndef AngelVM_VMImpl
#define AngelVM_VMImpl

typedef struct {
	uint8_t* ip;
	Value registers[VM_RegisterCount];
} AngelVM;

AngelVM CreateAVM() {
    AngelVM vm;
    vm.ip = NULL;
    return vm;
}

void RunFragment(AngelVM* vm, Fragment* frag) {
    vm->ip = frag->instructions.data;
    
    #define ReadByte() *vm->ip++
    #define ReadShort() Join(ReadByte(), ReadByte())

    for (;;) {
        for (int i = 0; i < VM_RegisterCount; i++) {
            if (i % 4 == 0) printf("|\n");
            printf("|r%02d = %16s", i, ValueAsString(&vm->registers[i]));
        }
        printf("|\n");
        DissasembleFragmentInstruction(frag, (int)(vm->ip - frag->instructions.data)); 
        uint8_t it = ReadByte();
        switch(it) {
            case OpMoveConstant: {
                uint16_t constIndex = ReadShort();
                uint8_t reg = ReadByte();
                Value* v = frag->constants.data[constIndex];
                vm->registers[reg].type = v->type;
                vm->registers[reg].as = v->as;
                break;
            }
            case OpMove: {
                uint8_t srcReg = ReadByte();
                uint8_t destReg = ReadByte();
                vm->registers[destReg] = vm->registers[srcReg];
                break;
            }
            case OpAdd: {
                uint8_t leftReg = ReadByte();
                uint8_t rightReg = ReadByte();
                uint8_t destReg = ReadByte();
                Value* a = &vm->registers[leftReg];
                Value* b = &vm->registers[rightReg];
                a = MakeCompatible(a, a->type < b->type ? a->type : b->type);
                b = MakeCompatible(b, a->type < b->type ? a->type : b->type); 
                if (a->type == ValueFloat)
                    a->as.fp32 = a->as.fp32 + b->as.fp32;
                else if (a->type == ValueDouble)
                    a->as.fp64 = a->as.fp64 + b->as.fp64;
                else 
                    a->as.int64 = a->as.int64 + b->as.int64;
                    
                vm->registers[destReg].as = a->as;
                vm->registers[destReg].type = a->type;
                break;
            }
            case OpReturn:
                printf("Fragment ended");
                return;
            default:
                printf("Unrecognized instruction: %d (possibly malformatted fragment)",
                    it);
                break;
        }
    }

    #undef ReadByte
}

#endif