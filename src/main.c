#include "AngelVM/VM.h"

#include <stdio.h>

int main(int argc, char** argv) {
    Fragment fragment;
    CreateFragment(&fragment);

    FragmentWriteConstant(&fragment, NewFloat(4.5), 0);
    FragmentWriteConstant(&fragment, NewFloat(7.5), 6);
    FragmentWriteConstant(&fragment, NewFloat(3.5), 2);
    FragmentWriteConstant(&fragment, NewFloat(1.5), 7);
    FragmentWrite(&fragment, OpReturn);

   // DissasembleFragment(&fragment);

    AngelVM vm = CreateAVM();

    RunFragment(&vm, &fragment);

    return 0;
}