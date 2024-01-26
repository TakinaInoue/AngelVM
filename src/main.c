#include "AngelVM/VM.h"

#include <stdio.h>

int main(int argc, char** argv) {
    Fragment fragment;
    CreateFragment(&fragment);

    FragmentWriteConstant(&fragment, NewFloat(4.5), 0);
    FragmentWriteConstant(&fragment, NewFloat(7.5), 1);
    FragmentWrite(&fragment, OpAdd);
    FragmentWrite(&fragment, 0);
    FragmentWrite(&fragment, 1);
    FragmentWrite(&fragment, 2);
    FragmentWrite(&fragment, OpReturn);

   // DissasembleFragment(&fragment);

    AngelVM vm = CreateAVM();

    RunFragment(&vm, &fragment);

    return 0;
}