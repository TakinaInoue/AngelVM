#include <stdint.h>

#ifndef AngelVM_Architecture
#define AngelVM_Architecture

#define VM_RegisterCount 16

#define OpReturn       0x0
#define OpMove         0x1
#define OpMoveConstant 0x02

void Split(uint16_t t, uint8_t* a, uint8_t* b) {
    *a = t >> 8;
    *b = t & 0x00FF;
}

uint16_t Join(uint8_t a, uint8_t b) {
    return ((a & 0xFF) << 8) | (b & 0xFF);
}

#endif