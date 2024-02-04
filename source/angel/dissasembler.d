module angel.dissasembler;

import core.stdc.stdio;
import core.stdc.stdlib;

import std.string;

import angel.arch;
import angel.memory;
import angel.bytecode;

void Dissasemble(Fragment fragment) {
    printf(" == AngelVM arch '%s' Fragment '%s.%s' == \n", AngelVM_Arch, fragment.sourceFile, fragment.name);
    printf("ConstantPool: %s", fragment.values);
    for (size_t i = 0; i < fragment.instructions.length;)
        i = DissasembleInst(i, fragment);
    writeln("== end ==");
}

size_t DissasembleInst(size_t offset, Fragment fragment) {
    printf(toLower(sprintf("%03d %03d %s ", offset,
        fragment.GetLine(offset),
        cast(OpSet)fragment.instructions[offset]
    )));

    switch(fragment.instructions[offset]) {
        case OpSet.Push:
        case OpSet.MemSize:
        case OpSet.Load:
        case OpSet.Pull:
        case OpSet.Jump:
        case OpSet.JumpBack:
            printf("%u\n", Join(fragment.instructions[offset+1], fragment.instructions[offset+2]));
            return offset + 3;
        default: printf("\n"); return offset+1;
    }
}