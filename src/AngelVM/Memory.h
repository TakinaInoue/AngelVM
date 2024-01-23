#include <stdint.h>

#ifndef AngelVM_Memory
#define AngelVM_Memory

#include <stdio.h>
#include <stdlib.h>

#define ValueFloat 0x1
#define ValueDouble 0x2

#define ValueInt8  0x10
#define ValueInt16 0x11
#define ValueInt32 0x12
#define ValueInt64 0x13

typedef struct Value {
    union {
        int8_t int8;
        int16_t int16;
        int32_t int32;
        int64_t int64;
        double fp64;
        float fp32;
    } as;
    int type;
} Value;

Value* NewValue() {
    return (Value*)malloc(sizeof(struct Value));
}

#define NewIntVal(NAME, TYPE, VALUETYPE) \
    Value* New##NAME(TYPE f) {\
        Value* v = NewValue();\
        v->as.int64 = f;\
        v->type = VALUETYPE;\
        return v;\
    } 

NewIntVal(Int8, int8_t, ValueInt8)
NewIntVal(Int16, int16_t, ValueInt16)
NewIntVal(Int32, int32_t, ValueInt32)
NewIntVal(Int64, int64_t, ValueInt64)

Value* NewDouble(double f) {
    Value* v = NewValue();
    v->as.fp64 = f;
    v->type = ValueDouble;
    return v;
}

Value* NewFloat(float f) {
    Value* v = NewValue();
    v->as.fp32 = f;
    v->type = ValueFloat;
    return v;
}

char* ValueAsString(Value* v) {
    char* outBuff = (char*) malloc(256 * sizeof(char));
    switch(v->type) {
        case ValueDouble: sprintf(outBuff, "%g", v->as.fp64); break;
        case ValueFloat: sprintf(outBuff, "%f", v->as.fp32); break;
        case ValueInt8:
        case ValueInt16:
        case ValueInt32:
        case ValueInt64:
            sprintf(outBuff, "%lld", v->as.int64);
            break;
    }
    return outBuff;
}

#define DEFINE_ARRAY(NAME, TYPE) \
    typedef struct { \
        size_t memSize;  \
        size_t length; \
        TYPE* data; \
    } NAME;\
    void Init##NAME(NAME* n) { \
        n->memSize = 0; \
        n->length = 0; \
        n->data = NULL; \
    } \
    void NAME##Write(NAME* array, TYPE value) { \
        if (array->length + 1 > array->memSize) { \
            int oldMemSize = array->memSize; \
            array->memSize = ((oldMemSize) < 8 ? 8 : (oldMemSize) * 2); \
            array->data = (TYPE*)reallocate(array->data, sizeof(TYPE) * (oldMemSize), sizeof(TYPE) * (array->memSize)); \
        } \
        array->data[array->length] = value;\
        array->length++; \
    }

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, newSize);
    return result;
}

DEFINE_ARRAY(ByteArray,uint8_t)
DEFINE_ARRAY(ValueArray, Value*);

#endif
