module main;

import std.stdio;
import core.stdc.time;

import angel.vm;

void main() {    
    Fragment fragment = new Fragment("entry");

    fragment.WriteValue(1, OpSet.Push, NewFloat(34));
    fragment.WriteValue(1, OpSet.Push, NewFloat(46));
    fragment.Write(2, OpSet.Add, OpSet.ResetStack, OpSet.Return);

    Dissasemble(fragment);
    
    AngelCore core = new AngelCore();
    time_t prev;
    time (&prev);
    uint[] ipsArray;
    while(ipsArray.length < 9) {
        time_t current;
        time (&current);
        core.RunFragment(fragment);
        if (current > prev) {
            prev = current;
            ipsArray ~= core.instructionsExecuted;
            core.instructionsExecuted = 0;
        }
    }
    uint total = 0;
    foreach(uint i ; ipsArray) {
        total += i;
    }
    total /= ipsArray.length;
    writeln("Average MIPS: ",  total / 1000000);
}