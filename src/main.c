#include "AngelVM/VM.h"

#include <stdio.h>

int main(int argc, char** argv) {
    Fragment fragment;
    CreateFragment(&fragment);

    FragmentWrite(&fragment, OpReturn);

    DissasembleFragment(&fragment);

    return 0;
}