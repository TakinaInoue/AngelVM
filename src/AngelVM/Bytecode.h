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

uint16_t FragmentWriteConstant(Fragment* fragment, Value* v) {
    ValueArrayWrite(&fragment->constants, v);
    return (uint16_t)(fragment->constants.length - 1);
}

void FragmentWrite(Fragment* fragment, uint8_t op) {
    ByteArrayWrite(&fragment->instructions, op);
}

#endif