#include "AngelVM.h"

int main() {
    Fragment* fragment = CreateFragment(); 
    FragmentWriteValue(fragment, 20, OpSet::Push, NewFloat(4.48));
    FragmentWriteValue(fragment, 20, OpSet::Push, NewFloat(2.42));
    FragmentWriteInst(fragment, 25, OpSet::Add);
    FragmentWriteInst(fragment, 30, OpSet::Return);

    DissasembleFragment(fragment);

    AngelCore* core = CreateAngelCore();

    CoreAddFrame(core, fragment);
    CoreExecute(core);

    return 0;
}