#include <stdint.h>

#ifndef AngelVM_Architecture
#define AngelVM_Architecture

#define VM_RegisterCount 8

//other
#define OpReturn       0x0
#define OpMove         0x01 // <ubyte - src> <ubyte - dest>
#define OpMoveConstant 0x02 // <ushort - src> <ubyte - dest>
//binary operations (all: <left - ubyte> <right - ubyte> <dest - ubyte>)
#define OpAdd 0x10
#define OpSub 0x11
#define OpMul 0x12
#define OpDiv 0x13
#define OpMod 0x14

void Split(uint16_t t, uint8_t* a, uint8_t* b) {
    *a = t >> 8;
    *b = t & 0x00FF;
}

uint16_t Join(uint8_t a, uint8_t b) {
    return ((a & 0xFF) << 8) | (b & 0xFF);
}

#endif