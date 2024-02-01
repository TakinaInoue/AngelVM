module angel.arch;

const string AngelVM_H1 = "H1";

const string AngelVM_Arch = AngelVM_H1; // or "Implementation Version"

enum OpSet {
    Push = 0x01,
    ResetStack = 0x02,
   //Pop  = 0x02, -- changed the way the stack works.
    
    Add = 0x10,
    Sub = 0x11,
    Div = 0x12,
    Mul = 0x13,
    Mod = 0x14,

    MemSize = 0x20,
    Load    = 0x21,
    Pull    = 0x22,

    Jump     = 0x30,
    JumpBack = 0x31,

    Return = 0xFF
}