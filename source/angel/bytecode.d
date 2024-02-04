module angel.bytecode;

import angel.memory;

class Application {
    
}

class Fragment {
    string sourceFile;
    string name;

    Line[] lines;
    Value[] values;
    ubyte[] instructions;

    this(string name, string sourceFile) {
        this.name = name;
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