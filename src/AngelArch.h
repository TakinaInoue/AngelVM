#pragma once

#define AngelVM_H0 0x000

#define AngelVM_Arch AngelVM_H0

typedef enum OpSet {
    Push = 0x01,
    Pop  = 0x02,   
    
    Add = 0x10,
    Sub = 0x11,
    Div = 0x12,
    Mul = 0x13,
    Mod = 0x14,

    Return = 0xFF
} OpSet;