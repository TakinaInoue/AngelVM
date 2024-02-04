module main;

import std.file;
import std.stdio;
import core.stdc.time;

import angel.vm;
import angeloscope.Parser;

void main() {    
    Parser parser = new Parser("test.ao", readText("test.ao"));
    writeln(parser.Parse() is null ? "Compilation Failed":"Okay.");

    AngelCore core = new AngelCore();


    /*
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
    writeln("Average MIPS: ",  total / 1000000);*/
}