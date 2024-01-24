#include <stdint.h>

#include "ARC.h"
#include "Memory.h"
#include "Bytecode.h"

#ifndef AngelVM_Core
#define AngelVM_Core

typedef struct CallFrame {
	struct CallFrame* prev;
	struct CallFrame* next;
	uint8_t* ip;

};

typedef struct {
	uint8_t* ip;
	CallFrame* currentFrame;
	Value r0, r1, r2, r3, r4, r5;
} AngelCore;

#endif
