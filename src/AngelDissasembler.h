#pragma once

#include "AngelCore.h"
#include "AngelBytecode.h"

size_t DissasembleInst(Fragment* frag, size_t offset);

void DissasembleFragment(Fragment* frag) {
    printf("== AngelVM (H%u Architecture) | Fragment ==\n", AngelVM_Arch);
    for (size_t d = 0; d < frag->values.length;d++) {
        printf("%zu. %s\n", d, ValueAsString(frag->values.data[d]));
    }
    for (size_t i = 0; i < frag->instructions.length;) {
        i = DissasembleInst(frag, i);
    }
    printf("== end of fragment ==");
}

size_t DissasembleInst(Fragment* frag, size_t offset) {
    printf("%03zu %03zd ", offset, FragmentGetLine(frag, offset));

#define disU16Inst(name) \
            printf("%s %u\n", name, Join(frag->instructions.data[offset + 1], frag->instructions.data[offset + 2]));\
            return offset + 3;\
    
    switch(frag->instructions.data[offset]) {
        case OpSet::Push: disU16Inst("push")  
        case OpSet::Add: printf("add\n"); return offset + 1;
        case OpSet::Sub: printf("sub\n"); return offset + 1;
        case OpSet::Mul: printf("mul\n"); return offset + 1;
        case OpSet::Div: printf("div\n"); return offset + 1;
        case OpSet::Mod: printf("mod\n"); return offset + 1;
        case OpSet::MemSize: disU16Inst("memsize")
        case OpSet::Load:    disU16Inst("load")
        case OpSet::Pull:    disU16Inst("pull")
        case OpSet::Return: printf("return\n"); return offset + 1;
        default:
            printf("unknown\n");
            return offset + 1;
    }
}