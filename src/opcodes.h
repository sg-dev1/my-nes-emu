#pragma once

#include "opcode.h"
#include "cpu.h"

#include <array>

constexpr Opcode make(
    const char* name,
    InstrFn_t operate,
    AddrModeFn_t addrmode,
    uint8_t cycles,
    bool extracCyclesOnPageCross
) {
    return Opcode{
        name,
        operate,
        addrmode,
        cycles,
        extracCyclesOnPageCross
    };
}

constexpr std::array<Opcode, 256> OPCODES = [] {

    std::array<Opcode, 256> table{};

    // Default: illegal opcode
    for (auto& op : table) {
        op = make("???", &Cpu::XXX, &Cpu::IMP, 2, false);
    }

    // LDA
    table[0xA9] = make("LDA", &Cpu::LDA, &Cpu::IMM, 2, false);
    table[0xA5] = make("LDA", &Cpu::LDA, &Cpu::ZP0, 3, false);
    table[0xB5] = make("LDA", &Cpu::LDA, &Cpu::ZPX, 4, false);
    table[0xAD] = make("LDA", &Cpu::LDA, &Cpu::ABS, 4, false);
    table[0xBD] = make("LDA", &Cpu::LDA, &Cpu::ABSX, 4, true);
    table[0xB9] = make("LDA", &Cpu::LDA, &Cpu::ABSY, 4, true);
    table[0xA1] = make("LDA", &Cpu::LDA, &Cpu::IDX, 6, false);
    table[0xB1] = make("LDA", &Cpu::LDA, &Cpu::IDY, 5, true);

    // LDX
    table[0xA2] = make("LDX", &Cpu::LDX, &Cpu::IMM, 2, false);
    table[0xA6] = make("LDX", &Cpu::LDX, &Cpu::ZP0, 3, false);
    table[0xB6] = make("LDX", &Cpu::LDX, &Cpu::ZPY, 4, false);
    table[0xAE] = make("LDX", &Cpu::LDX, &Cpu::ABS, 4, false);
    table[0xBE] = make("LDX", &Cpu::LDX, &Cpu::ABSY, 4, true);

    // LDY
    table[0xA0] = make("LDY", &Cpu::LDY, &Cpu::IMM, 2, false);
    table[0xA4] = make("LDY", &Cpu::LDY, &Cpu::ZP0, 3, false);
    table[0xB4] = make("LDY", &Cpu::LDY, &Cpu::ZPX, 4, false);
    table[0xAC] = make("LDY", &Cpu::LDY, &Cpu::ABS, 4, false);
    table[0xBC] = make("LDY", &Cpu::LDY, &Cpu::ABSX, 4, true);

    // STA
    table[0x85] = make("STA", &Cpu::STA, &Cpu::ZP0, 3, false);
    table[0x95] = make("STA", &Cpu::STA, &Cpu::ZPX, 4, false);
    table[0x8D] = make("STA", &Cpu::STA, &Cpu::ABS, 4, false);
    table[0x9D] = make("STA", &Cpu::STA, &Cpu::ABSX, 5, false);
    table[0x99] = make("STA", &Cpu::STA, &Cpu::ABSY, 5, false);
    table[0x81] = make("STA", &Cpu::STA, &Cpu::IDX, 6, false);
    table[0x91] = make("STA", &Cpu::STA, &Cpu::IDY, 6, false);

    // TODO

    return table;
}();