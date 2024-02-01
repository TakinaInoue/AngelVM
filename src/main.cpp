#include "AngelVM.h"

int main() {
    Fragment* fragment = CreateFragment(); 

    uint8_t aA, bA;
    uint8_t aB, bB;
    uint8_t aS, bS;
    Split(2, &aS, &bS);
    Split(0, &aA, &bA);
    Split(1, &aB, &bB);

    FragmentWriteValue(fragment, 20, OpSet::Push, NewFloat(4.48));
    FragmentWriteValue(fragment, 20, OpSet::Push, NewFloat(2.42));
    FragmentWriteInst(fragment, 25, OpSet::MemSize, aS, bS);
    
    FragmentWriteInst(fragment, 25, OpSet::Load, aA, bA);
    FragmentWriteInst(fragment, 25, OpSet::Load, aB, bB);

    FragmentWriteInst(fragment, 25, OpSet::Pull, aA, bA);
    FragmentWriteInst(fragment, 25, OpSet::Pull, aB, bB);

    FragmentWriteInst(fragment, 25, OpSet::Add);
    FragmentWriteInst(fragment, 30, OpSet::Return);

    DissasembleFragment(fragment);

    AngelCore* core = CreateAngelCore();

    CoreAddFrame(core, fragment);
    CoreExecute(core);

    return 0;
}