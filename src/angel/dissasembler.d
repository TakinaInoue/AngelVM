module angel.dissasembler;

import std.stdio;
import std.format;
import std.string;

import angel.arch;
import angel.memory;
import angel.bytecode;

void Dissasemble(Fragment fragment) {
    writefln(" == AngelVM arch '%s' Fragment '%s' == ", AngelVM_Arch, fragment.name);
    writeln("ConstantPool: ", fragment.values);
    for (size_t i = 0; i < fragment.instructions.length;)
        i = DissasembleInst(i, fragment);
    writeln("== end ==");
}

size_t DissasembleInst(size_t offset, Fragment fragment) {
    write(toLower(format("%03d %03d %s ", offset,
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
            writeln(Join(fragment.instructions[offset+1], fragment.instructions[offset+2]));
            return offset + 3;
        default: writeln(); return offset+1;
    }
}