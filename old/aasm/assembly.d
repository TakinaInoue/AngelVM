module angel.aasm.assembly;

import std.stdio;
import std.string;
import std.conv : to;

import core.stdc.ctype : toupper;

import angel.arch;
import angel.memory;
import angel.bytecode;

Fragment AssembleFragment(string file, string filename, size_t startLine) {
    void err(string msg, size_t line) {
        writeln("Error in line ", startLine + line, " in file ", filename, " > ", msg);
    }
    string[] lines = file.split("\n");
    if (lines.length == 0) {
        err("Bad Fragment", 1);
        return null;
    }
    string defLine = lines[0].strip();
    if (defLine.length == 0 || defLine[0] != '.' || defLine[defLine.length - 1] != ':') {
        err("Bad fragment: missing status line.", 1);
        return null;
    }
    defLine = defLine[1 .. defLine.length - 1];
    if (defLine.length == 0) {
        err("Bad fragment name.", 1);
        return null;
    }
    
    Fragment fragment = new Fragment(defLine);
    ushort[string] constants;
    ushort[string] locals;
    int stackValueCount = 0;
    int maxLocalCount = 0;
    size_t i = 1;
    for (; i < lines.length; i++) {
        string line = lines[i].strip();
        string[] tokens = line.split(" ");
        if (tokens.length == 0) continue;
        switch(tokens[0]) {
            case "dv": {
                if (tokens.length < 4) {
                    err("usage: dv [name] [type] [value...]", i+1);
                    return null;
                }
                ValueType t = tokens[2].to!ValueType;
                Value* v;
                switch(t) {
                    case ValueType.Byte: v = NewByte(tokens[3].to!byte); break;
                    case ValueType.Short: v = NewShort(tokens[3].to!short); break;
                    case ValueType.Int: v = NewInt(tokens[3].to!int); break;
                    case ValueType.Long: v = NewLong(tokens[3].to!long); break;
                    case ValueType.Float: v = NewFloat(tokens[3].to!float); break;
                    case ValueType.Double: v = NewDouble(tokens[3].to!double); break;
                    default: throw new Exception("Incomplete Implementation.");
                }
                constants[tokens[1]] = cast(ushort)fragment.values.length;
                fragment.values ~= *v;  
                break;
            }
            case "push": {
                if (tokens.length < 2) {
                    err("Expected a constant name, push [name]", i+1);
                    return null;
                }
                ushort* us = tokens[1] in constants;
                if (us == null) {
                    err("Unknown constant: "~tokens[1], i+1);
                    return null;
                }
                ubyte a, b;
                Split(*us, a, b);
                fragment.Write(i+1, OpSet.Push, a, b);
                stackValueCount++;
                break;
            }
            case "restk":
                fragment.Write(i+1, OpSet.ResetStack);
                break;
            case "add":
            case "sub":
            case "div":
            case "mul":
            case "mod": {
                if (stackValueCount < 2) {
                    err("This instruction requires at least 2 operands on the stack.", i + 1);
                    return null;
                }
                string op = tokens[0];
                op = cast(char)op[0].toupper() ~ op[1 .. op.length];
                fragment.Write(i+1, op.to!OpSet);
                break;
            } 
            case "msize": {
                if (tokens.length < 2) {
                    err("This instruction requires a 16 bit operand.", i+1);
                    return null;
                }
                ushort ee = tokens[1].to!ushort;
                ubyte a, b;
                maxLocalCount = ee;
                Split(ee, a, b);
                fragment.Write(i+1, OpSet.MemSize, a, b);
                break;
            }
            case "load": {
                if (tokens.length < 2) {
                    err("This instruction requires a name for it's local value.", i+1);
                    return null;
                }
                if (stackValueCount < 1) {
                    err("This instruction requires at least 1 operand on the stack.", i + 1);
                    return null;
                }
                string name = tokens[1];
                ushort* loc = name in locals;
                if (loc == null) {
                    locals[name] = cast(ushort)locals.length;
                    loc = name in locals;
                }
                ubyte a, b;
                Split(*loc, a, b);
                fragment.Write(i+1, OpSet.Load, a, b);
                break;
            }
            case "pull": {
                if (tokens.length < 2) {
                    err("This instruction requires a name for it's local value.", i+1);
                    return null;
                }
                string name = tokens[1];
                ushort* loc = name in locals;
                if (loc == null) {
                    err("Undefined local", i + 1);
                    return null;
                }
                ubyte a, b;
                Split(*loc, a, b);
                fragment.Write(i+1, OpSet.Pull, a, b);
                break;
            }
            case "ret":
                stackValueCount = 0;
                fragment.Write(i+1, OpSet.Return);
                if (locals.length > maxLocalCount) {
                    writefln("Memory Size was set to %d in fragment %s, but the program uses %d spaces.",
                                maxLocalCount, fragment.name, locals.length);
                    return null;
                }
                return fragment;
            default: err("Unknown Operation", i+1); return null;
        }
    }
    err("Missing 'return' instruction.", i);
    return null;
}