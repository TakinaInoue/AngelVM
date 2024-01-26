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
        printf("%s %d %d \n", name, fragment->instructions.data[offset+1], fragment->instructions.data[offset+2]); \
        return offset + 3;

    #define ArithDissOp(name) \
            printf("%s %d %d to %d\n", name, fragment->instructions.data[offset + 1], fragment->instructions.data[offset + 2], fragment->instructions.data[offset + 3]); \
            return offset + 4; 
 
    switch(fragment->instructions.data[offset]) {
        case OpMove: 
            printf("move %d -> %d\n", fragment->instructions.data[offset + 1], fragment->instructions.data[offset + 2]);
            return offset + 3;
        case OpAdd: ArithDissOp("add")
        case OpSub: ArithDissOp("sub")
        case OpMul: ArithDissOp("mul")
        case OpDiv: ArithDissOp("div")
        case OpMod: ArithDissOp("mod")
        case OpMoveConstant: 
            printf("move-constant %u -> %d \n", Join(fragment->instructions.data[offset+1], fragment->instructions.data[offset+2]), fragment->instructions.data[offset+3]);
            return offset + 4;
        case OpReturn: SingleOperandOp("return")
        default:
            printf("unknown_op\n");
            return offset + 1;
    }
    
    #undef ArithDissOp
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