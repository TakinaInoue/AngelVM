module angel.memory;

import std.format;

struct Line {
    size_t line;
    size_t endOffset;
}

enum ValueType {
    Byte  = 0x1,
    Short = 0x2,
    Int   = 0x3,
    Long  = 0x4,

    Float = 0x5,
    Double = 0x6
}

struct Value {
    size_t vtype;
    as_ as;
    union as_ {
        byte i8;
        short i16;
        int i32;
        long i64;
        
        float fp32;
        double fp64;
    }

    string toString() const @safe pure
    {
        if (vtype <= ValueType.Long)
            return format("%d", as.i64);
        else if (vtype == ValueType.Float)
            return format("%f", as.fp32);
        else if (vtype == ValueType.Double)
            return format("%g", as.fp64);
        return "unknown";
    }
}

Value* NewValue() {
    return new Value();
}

Value* NewByte(byte b) {
    auto d = NewValue();
    d.as.i8 = b;
    d.vtype = ValueType.Byte;
    return d;
}

Value* NewShort(short b) {
    auto d = NewValue();
    d.as.i16 = b;
    d.vtype = ValueType.Short;
    return d;
}

Value* NewInt(int b) {
    auto d = NewValue();
    d.as.i32 = b;
    d.vtype = ValueType.Int;
    return d;
}

Value* NewLong(long b) {
    auto d = NewValue();
    d.as.i64 = b;
    d.vtype = ValueType.Long;
    return d;
}

Value* NewFloat(float b) {
    auto d = NewValue();
    d.as.fp32 = b;
    d.vtype = ValueType.Float;
    return d;
}

Value* NewDouble(double b) {
    auto d = NewValue();
    d.as.fp64 = b;
    d.vtype = ValueType.Double;
    return d;
}

void Convert(ref Value src, size_t desired) {
    if (src.vtype == desired) 
        return;
    else if (desired == ValueType.Float) {
        if (src.vtype <= ValueType.Long) 
            src.as.fp32 = cast(float) src.as.i64;
        else
            src.as.fp32 = cast(float) src.as.fp64;
    } else if (desired == ValueType.Double) {
        if (src.vtype <= ValueType.Long) 
            src.as.fp64 = cast(double) src.as.i64;
        else
            src.as.fp64 = cast(double) src.as.fp32;
    } else if (desired <= ValueType.Long) {
        if (src.vtype == ValueType.Float)
            src.as.i64 = cast(long) src.as.fp32;
        else
            src.as.i64 = cast(long) src.as.fp64;
    }
    src.vtype = desired;
}

void Split(ushort t, ref ubyte a, ref ubyte b) {
    version(BigEndian) {
        a = (t << 8) & 0xFF;
        b = t & 0xFF;
    } else {
        a = t & 0xFF;
        b = (t << 8) & 0xFF;
    }
}

ushort Join(ubyte a, ubyte b) {
    version(BigEndian) {
        return (a >> 8) & 0xFF | b & 0xFF;
    } else {
        return a & 0xFF | (b >> 8) & 0xFF;
    }
}