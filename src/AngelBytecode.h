#pragma once

#include <stdint.h>

#include "AngelCore.h"
#include "AngelMemory.h"

void Split(uint16_t t, uint8_t* a, uint8_t* b) {
    *a = t >> 8;
    *b = t & 0x00FF;
}

uint16_t Join(uint8_t a, uint8_t b) {
    return ((a & 0xFF) << 8) | (b & 0xFF);
}

typedef struct Fragment {
    ByteArray instructions;
    ValueArray values;
    LineArray lines;
} Fragment;

Fragment* CreateFragment() {
    Fragment* frag = (Fragment*) malloc(sizeof(struct Fragment));
    InitByteArray(&frag->instructions);
    InitLineArray(&frag->lines);
    InitValueArray(&frag->values);
    return frag;   
}

using uint8_tarr = uint8_t[];
#define FragmentWriteInst(fragment, line, ...) FragmentWriteInst_(fragment, line, sizeof(uint8_tarr { __VA_ARGS__ }), (uint8_tarr { __VA_ARGS__ }))

void FragmentWriteInst_(Fragment* fragment, size_t line, size_t opcodeCount, uint8_t* opcodes) {
    if (fragment->lines.length > 0 && fragment->lines.data[fragment->lines.length - 1]->line == line) {
        fragment->lines.data[fragment->lines.length - 1]->offset += opcodeCount;
    } else {
        LineArrayWrite(&fragment->lines, NewLine(fragment->instructions.length + opcodeCount, line));
    }

    for (size_t i = 0; i < opcodeCount; i++) {
                                                // probably should do this in a better way
        ByteArrayWrite(&fragment->instructions, opcodes[i]);
    }
}

void FragmentWriteValue(Fragment* fragment, size_t line, uint8_t op, Value* value) {
    ValueArrayWrite(&fragment->values,  value);
    uint16_t offs = (uint16_t) (fragment->values.length - 1);
    uint8_t a, b;
    Split(offs, &a, &b);
    FragmentWriteInst(fragment, line, op, a, b);
}

size_t FragmentGetLine(Fragment* fragment, size_t offset) {
    size_t lastLine = 0;
    for (size_t i = 0; i < fragment->lines.length; i++) {
        Line* l = fragment->lines.data[i];
        if (l->offset >= offset)
            return lastLine == 0 ? l->line : lastLine;
        lastLine = l->line;
    }
    return lastLine;
}