#include "ARC.h"
#include "Memory.h"

#ifndef AngelVM_Bytecode
#define AngelVM_Bytecode

typedef struct {
    ValueArray constants;
    ByteArray instructions;
} Fragment;

void CreateFragment(Fragment* fragment) {
    InitValueArray(&fragment->constants);
    InitByteArray(&fragment->instructions);
} 

void FragmentWrite(Fragment* fragment, uint8_t op) {
    ByteArrayWrite(&fragment->instructions, op);
}

void FragmentWriteConstant(Fragment* fragment, Value* v, uint8_t rg) {
    if (rg >= VM_RegisterCount) {
        printf("This VM has %u registers, but bytecode is trying to use register %d.", VM_RegisterCount, rg);
        exit(599000);
    }
    ValueArrayWrite(&fragment->constants, v);
    uint8_t a, b;
    Split((uint16_t)(fragment->constants.length - 1), &a, &b);
    FragmentWrite(fragment, OpMoveConstant);
    FragmentWrite(fragment, a);
    FragmentWrite(fragment, b);
    FragmentWrite(fragment, rg);
}

#endif