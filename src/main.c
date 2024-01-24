#include "AngelVM/VM.h"

#include <stdio.h>

int main(int argc, char** argv) {
    Fragment fragment;
    CreateFragment(&fragment);

    FragmentWrite(&fragment, OpMove);
    FragmentWriteConstant(&fragment, NewFloat(4.5));
    FragmentWrite(&fragment, OpReturn);

    DissasembleFragment(&fragment);

    return 0;
}