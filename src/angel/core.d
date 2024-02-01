module angel.core;

import std.format;

import angel.vm;
import core.memory : GC;

struct CallFrame {
    ubyte* ip;
    Value[] locals;
    Fragment fragment;
    CallFrame* next;
    CallFrame* prev;
}

final class AngelCore {
    CallFrame* callStackTop;

    Value[32] stack;
    Value* stackTop;

    uint instructionsExecuted;

    this() {
        instructionsExecuted = 0;
        Reset();
    }

    void Reset() {
        stackTop = stack.ptr;
        GC.free(callStackTop);
        callStackTop = null;
    }

    void RunFragment(Fragment fragment) {
        if (!callStackTop)
            callStackTop = new CallFrame();
        else  {
            if (!callStackTop.next)
                callStackTop.next = new CallFrame();
            auto prev = callStackTop;
            callStackTop = callStackTop.next;
            callStackTop.prev = prev;
        } 
        callStackTop.fragment = fragment;
        callStackTop.ip = fragment.instructions.ptr;
        Execute();
    }

    ubyte ReadByte() {return *callStackTop.ip++;}
    ushort ReadShort() {return Join(ReadByte(), ReadByte());}

    void Push(Value v) {
        *stackTop++ = v;
    }

    Value StackGet(size_t dst) {
        return stackTop[-dst];
    }

    void Execute() {
        for (;;) {
            debug {
                import std.stdio;
                for (Value* p = stack.ptr; p < stackTop; p++) {
                    write(*p, " ");
                }
                writeln();
                DissasembleInst(cast(size_t)(callStackTop.ip - callStackTop.fragment.instructions.ptr), callStackTop.fragment);
            }
            ubyte it = ReadByte();
            switch(it) {
                case OpSet.Push: {
                    Push(callStackTop.fragment.values[ReadShort()]);
                    break;
                }
                case OpSet.ResetStack: {
                    stackTop = stack.ptr;
                    break;
                }
                mixin(ArithInst!("Add","+"));
                mixin(ArithInst!("Sub","-"));
                mixin(ArithInst!("Mul","*"));
                mixin(ArithInst!("Div","/"));
                mixin(ArithInst!("Mod","%"));
                case OpSet.MemSize: {
                    ushort sz = ReadShort();
                    if (callStackTop.locals.length < sz) {
                        callStackTop.locals = new Value[sz];
                    }
                    break;
                }
                case OpSet.Load: {
                    ushort li = ReadShort();
                    callStackTop.locals[li] = StackGet(0);
                    break;
                }
                case OpSet.Pull: {
                    ushort li = ReadShort();
                    Push(callStackTop.locals[li]);
                    break;
                }
                case OpSet.Jump: {
                    callStackTop.ip += ReadShort();
                    break;
                }
                case OpSet.JumpBack: {
                    callStackTop.ip -= ReadShort();
                    break;
                }
                case OpSet.Return: {
                    callStackTop = callStackTop.prev;
                    return;
                }
                default: throw new Exception(
                    format("Unknown opcode %X likely poor implementation or poor bytecode",it));
            }
            instructionsExecuted++;
        }
    }
}   

private template ArithInst(string op, string ch) {
    const char[] ArithInst = "
        case OpSet."~op~": {
            Value a = StackGet(2);
            Value b = StackGet(1);
            size_t resType = a.vtype > b.vtype ? a.vtype : b.vtype;

            Convert(a, resType);
            Convert(b, resType);

            if (a.vtype <= ValueType.Long)
                a.as.i64 = a.as.i64 "~ch~" b.as.i64;
            else if (a.vtype == ValueType.Float)
                a.as.fp32 = a.as.fp32 "~ch~" b.as.fp32;
            else
                a.as.fp64 = a.as.fp64 "~ch~" b.as.fp64;
            Push(a);
            break;
        }
    ";
}