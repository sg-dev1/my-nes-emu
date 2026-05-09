#pragma once
#include <cstdint>

class Cpu;

struct AddrResult 
{
    uint16_t addr;
    bool pageCrossed;
};

using AddrModeFn_t = AddrResult (Cpu::*)();
using InstrFn_t = void (Cpu::*)(uint16_t);

struct Opcode {
    const char* name;

    InstrFn_t instr;
    AddrModeFn_t addrmode;

    uint8_t cycles;
    bool extracCyclesOnPageCross;
};
