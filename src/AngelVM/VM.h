#include "ARC.h"
#include "Bytecode.h"
#include "Dissasembler.h"

#ifndef AngelVM_VMImpl
#define AngelVM_VMImpl

typedef struct {
	uint8_t* ip;
	Value registers[VM_RegisterCount];
} AngelVM;

AngelVM CreateAVM() {
    AngelVM vm;
    vm.ip = NULL;
    return vm;
}

void RunFragment(AngelVM* vm, Fragment* frag) {
    vm->ip = frag->instructions.data;
    
    #define ReadByte() *vm->ip++
    #define ReadShort() Join(ReadByte(), ReadByte())

    for (;;) {
        for (int i = 0; i < VM_RegisterCount; i++) {
            if (i % 6 == 0) printf("|\n");
            printf("|r%02d = %16s", i, ValueAsString(&vm->registers[i]));
        }
        printf("|\n");
        DissasembleFragmentInstruction(frag, (int)(vm->ip - frag->instructions.data)); 
        uint8_t it = ReadByte();
        switch(it) {
            case OpMoveConstant: {
                uint16_t constIndex = ReadShort();
                uint8_t reg = ReadByte();
                Value* v = frag->constants.data[constIndex];
                vm->registers[reg].type = v->type;
                vm->registers[reg].as = v->as;
                break;
            }
            case OpMove: {
                uint8_t srcReg = ReadByte();
                uint8_t destReg = ReadByte();
                vm->registers[destReg] = vm->registers[srcReg];
                break;
            }
            case OpReturn:
                printf("Fragment ended");
                return;
            default:
                printf("Unrecognized instruction: %d (possibly malformatted fragment)",
                    it);
                break;
        }
    }

    #undef ReadByte
}

#endif