module angel.bytecode;

import angel.memory;

class Fragment {
    string name;

    Line[] lines;
    Value[] values;
    ubyte[] instructions;

    this(string name) {
        this.name = name;
    }

    void Write(int line, ubyte[] inst...) {
        if (lines.length > 0 && lines[lines.length - 1].line == line) {
            lines[lines.length -1].endOffset += inst.length;
        } else {
            lines ~= Line(line, instructions.length);
        }
        instructions ~= inst;
    }

    void WriteU16(int line, ushort d) {
        ubyte a, b; Split(d, a, b);
        Write(line, a, b);
    }

    void WriteValue(int line, ubyte opcode, Value* v) {
        values ~= Value(v.vtype, v.as);

        ubyte a, b; Split(cast(ushort)(values.length - 1), a, b);
        Write(line, opcode, a, b);
    }

    size_t GetLine(size_t offs) {
        size_t lastLine = 0;
        for (size_t i = 0; i < lines.length; i++) {
            if (lines[i].endOffset > offs) {
                return lastLine == 0 ? lines[i].line : lastLine;
            }
            lastLine = lines[i].line;
        }
        return lastLine;
    }
}