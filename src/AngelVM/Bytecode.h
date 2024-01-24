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

void FragmentWriteConstant(Fragment* fragment, Value* v) {
    ValueArrayWrite(&fragment->constants, v);
    uint8_t a, b;
    Split((uint16_t)(fragment->constants.length - 1), &a, &b);
    FragmentWrite(fragment, a);
    FragmentWrite(fragment, b);
}

#endif