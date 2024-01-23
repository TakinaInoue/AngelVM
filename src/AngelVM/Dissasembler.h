#include "stdio.h"

#include "Bytecode.h"

#ifndef AngelVM_Dissasembler
#define AngelVM_Dissasembler

int DissasembleFragmentInstruction(Fragment* fragment, int offset) {
    printf("%03d ", offset);

    #define SingleOperandOp(name) \
        printf("%s\n", name); \
        return offset + 1;

    switch(fragment->instructions.data[offset]) {
        case OpReturn: SingleOperandOp("return")
        default:
            printf("unknown_op\n");
            return offset + 1;

    }
    #undef SingleOperandOp
}

void DissasembleFragment(Fragment* fragment) {
    for (int i = 0; i < fragment->instructions.length;) {
        i = DissasembleFragmentInstruction(fragment, i);
    }
}

#endif