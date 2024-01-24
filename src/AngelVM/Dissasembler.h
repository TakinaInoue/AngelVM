#include "stdio.h"

#include "Bytecode.h"

#ifndef AngelVM_Dissasembler
#define AngelVM_Dissasembler

int DissasembleFragmentInstruction(Fragment* fragment, int offset) {
    printf("%03d ", offset);

    #define SingleOperandOp(name) \
        printf("%s\n", name); \
        return offset + 1;
    #define U16Op(name) \
        printf("%s %u\n", name, (fragment->instructions.data[offset+1], fragment->instructions.data[offset+2])); \
        return offset + 3;\
 
    switch(fragment->instructions.data[offset]) {
        case OpMove: U16Op("move");
        case OpReturn: SingleOperandOp("return")
        default:
            printf("unknown_op\n");
            return offset + 1;
    }
    
    #undef u16Op
    #undef SingleOperandOp
}

void DissasembleFragment(Fragment* fragment) {
    printf(" == constants: \n");

    for (int k = 0; k < fragment->constants.length; k++) {
        printf("\t%s\n",ValueAsString(fragment->constants.data[k]));
    }

    printf(" == instructions: \n");
    
    for (int i = 0; i < fragment->instructions.length;) {
        i = DissasembleFragmentInstruction(fragment, i);
    }
}

#endif