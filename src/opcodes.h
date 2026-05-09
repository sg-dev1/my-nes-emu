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

    // ADC
    table[0x69] = make("ADC", &Cpu::ADC, &Cpu::IMM, 2, false);
    table[0x65] = make("ADC", &Cpu::ADC, &Cpu::ZP0, 3, false);
    table[0x75] = make("ADC", &Cpu::ADC, &Cpu::ZPX, 4, false);
    table[0x6D] = make("ADC", &Cpu::ADC, &Cpu::ABS, 4, false);
    table[0x7D] = make("ADC", &Cpu::ADC, &Cpu::ABSX, 4, true);
    table[0x79] = make("ADC", &Cpu::ADC, &Cpu::ABSY, 4, true);
    table[0x61] = make("ADC", &Cpu::ADC, &Cpu::IDX, 6, false);
    table[0x71] = make("ADC", &Cpu::ADC, &Cpu::IDY, 5, true);

    // AND
    table[0x29] = make("AND", &Cpu::AND, &Cpu::IMM, 2, false);
    table[0x25] = make("AND", &Cpu::AND, &Cpu::ZP0, 3, false);
    table[0x35] = make("AND", &Cpu::AND, &Cpu::ZPX, 4, false);
    table[0x2D] = make("AND", &Cpu::AND, &Cpu::ABS, 4, false);
    table[0x3D] = make("AND", &Cpu::AND, &Cpu::ABSX, 4, true);
    table[0x39] = make("AND", &Cpu::AND, &Cpu::ABSY, 4, true);
    table[0x21] = make("AND", &Cpu::AND, &Cpu::IDX, 6, false);
    table[0x31] = make("AND", &Cpu::AND, &Cpu::IDY, 5, true);

    // ASL
    table[0x0A] = make("ASL", &Cpu::ASL, &Cpu::IMP, 2, false);
    table[0x06] = make("ASL", &Cpu::ASL, &Cpu::ZP0, 5, false);
    table[0x16] = make("ASL", &Cpu::ASL, &Cpu::ZPX, 6, false);
    table[0x0E] = make("ASL", &Cpu::ASL, &Cpu::ABS, 6, false);
    table[0x1E] = make("ASL", &Cpu::ASL, &Cpu::ABSX, 7, false);

    // BCC
    table[0x90] = make("BCC", &Cpu::BCC, &Cpu::REL, 2, true);

    // BCS
    table[0xB0] = make("BCS", &Cpu::BCS, &Cpu::REL, 2, true);

    // BEQ
    table[0xF0] = make("BEQ", &Cpu::BEQ, &Cpu::REL, 2, true);

    // BIT
    table[0x24] = make("BIT", &Cpu::BIT, &Cpu::ZP0, 3, false);
    table[0x2C] = make("BIT", &Cpu::BIT, &Cpu::ABS, 4, false);

    // BMI
    table[0x30] = make("BMI", &Cpu::BMI, &Cpu::REL, 2, true);

    // BNE
    table[0xD0] = make("BNE", &Cpu::BNE, &Cpu::REL, 2, true);

    // BPL
    table[0x10] = make("BPL", &Cpu::BPL, &Cpu::REL, 2, true);

    // BRK
    table[0x00] = make("BRK", &Cpu::BRK, &Cpu::IMM, 7, false);

    // BVC
    table[0x50] = make("BVC", &Cpu::BVC, &Cpu::REL, 2, true);

    // BVS
    table[0x70] = make("BVS", &Cpu::BVS, &Cpu::REL, 2, true);

    // CLC
    table[0x18] = make("CLC", &Cpu::CLC, &Cpu::IMP, 2, false);

    // CLD
    table[0xD8] = make("CLD", &Cpu::CLD, &Cpu::IMP, 2, false);

    // CLI
    table[0x58] = make("CLI", &Cpu::CLI, &Cpu::IMP, 2, false);

    // CLV
    table[0xB8] = make("CLV", &Cpu::CLV, &Cpu::IMP, 2, false);

    // CMP
    table[0xC9] = make("CMP", &Cpu::CMP, &Cpu::IMM, 2, false);
    table[0xC5] = make("CMP", &Cpu::CMP, &Cpu::ZP0, 3, false);
    table[0xD5] = make("CMP", &Cpu::CMP, &Cpu::ZPX, 4, false);
    table[0xCD] = make("CMP", &Cpu::CMP, &Cpu::ABS, 4, false);
    table[0xDD] = make("CMP", &Cpu::CMP, &Cpu::ABSX, 4, true);
    table[0xD9] = make("CMP", &Cpu::CMP, &Cpu::ABSY, 4, true);
    table[0xC1] = make("CMP", &Cpu::CMP, &Cpu::IDX, 6, false);
    table[0xD1] = make("CMP", &Cpu::CMP, &Cpu::IDY, 5, true);

    // CPX
    table[0xE0] = make("CPX", &Cpu::CPX, &Cpu::IMM, 2, false);
    table[0xE4] = make("CPX", &Cpu::CPX, &Cpu::ZP0, 3, false);
    table[0xEC] = make("CPX", &Cpu::CPX, &Cpu::ABS, 4, false);

    // CPY
    table[0xC0] = make("CPY", &Cpu::CPY, &Cpu::IMM, 2, false);
    table[0xC4] = make("CPY", &Cpu::CPY, &Cpu::ZP0, 3, false);
    table[0xCC] = make("CPY", &Cpu::CPY, &Cpu::ABS, 4, false);

    // DEC
    table[0xC6] = make("DEC", &Cpu::DEC, &Cpu::ZP0, 5, false);
    table[0xD6] = make("DEC", &Cpu::DEC, &Cpu::ZPX, 6, false);  
    table[0xCE] = make("DEC", &Cpu::DEC, &Cpu::ABS, 6, false);
    table[0xDE] = make("DEC", &Cpu::DEC, &Cpu::ABSX, 7, false);

    // DEX
    table[0xCA] = make("DEX", &Cpu::DEX, &Cpu::IMP, 2, false);

    // DEY
    table[0x88] = make("DEY", &Cpu::DEY, &Cpu::IMP, 2, false);

    // EOR
    table[0x49] = make("EOR", &Cpu::EOR, &Cpu::IMM, 2, false);
    table[0x45] = make("EOR", &Cpu::EOR, &Cpu::ZP0, 3, false);
    table[0x55] = make("EOR", &Cpu::EOR, &Cpu::ZPX, 4, false);
    table[0x4D] = make("EOR", &Cpu::EOR, &Cpu::ABS, 4, false);
    table[0x5D] = make("EOR", &Cpu::EOR, &Cpu::ABSX, 4, true);
    table[0x59] = make("EOR", &Cpu::EOR, &Cpu::ABSY, 4, true);
    table[0x41] = make("EOR", &Cpu::EOR, &Cpu::IDX, 6, false);
    table[0x51] = make("EOR", &Cpu::EOR, &Cpu::IDY, 5, true);

    // INC
    table[0xE6] = make("INC", &Cpu::INC, &Cpu::ZP0, 5, false);
    table[0xF6] = make("INC", &Cpu::INC, &Cpu::ZPX, 6, false);
    table[0xEE] = make("INC", &Cpu::INC, &Cpu::ABS, 6, false);
    table[0xFE] = make("INC", &Cpu::INC, &Cpu::ABSX, 7, false);

    // INX
    table[0xE8] = make("INX", &Cpu::INX, &Cpu::IMP, 2, false);

    // INY
    table[0xC8] = make("INY", &Cpu::INY, &Cpu::IMP, 2, false);

    // JMP
    table[0x4C] = make("JMP", &Cpu::JMP, &Cpu::ABS, 3, false);
    table[0x6C] = make("JMP", &Cpu::JMP, &Cpu::IND, 5, false);

    // JSR
    table[0x20] = make("JSR", &Cpu::JSR, &Cpu::ABS, 6, false);

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

    // LSR
    table[0x4A] = make("LSR", &Cpu::LSR, &Cpu::IMP, 2, false);
    table[0x46] = make("LSR", &Cpu::LSR, &Cpu::ZP0, 5, false);
    table[0x56] = make("LSR", &Cpu::LSR, &Cpu::ZPX, 6, false);
    table[0x4E] = make("LSR", &Cpu::LSR, &Cpu::ABS, 6, false);
    table[0x5E] = make("LSR", &Cpu::LSR, &Cpu::ABSX, 7, false);

    // NOP
    table[0xEA] = make("NOP", &Cpu::NOP, &Cpu::IMP, 2, false);

    // ORA
    table[0x09] = make("ORA", &Cpu::ORA, &Cpu::IMM, 2, false);
    table[0x05] = make("ORA", &Cpu::ORA, &Cpu::ZP0, 3, false);
    table[0x15] = make("ORA", &Cpu::ORA, &Cpu::ZPX, 4, false);
    table[0x0D] = make("ORA", &Cpu::ORA, &Cpu::ABS, 4, false);
    table[0x1D] = make("ORA", &Cpu::ORA, &Cpu::ABSX, 4, true);
    table[0x19] = make("ORA", &Cpu::ORA, &Cpu::ABSY, 4, true);
    table[0x01] = make("ORA", &Cpu::ORA, &Cpu::IDX, 6, false);
    table[0x11] = make("ORA", &Cpu::ORA, &Cpu::IDY, 5, true);

    // PHA
    table[0x48] = make("PHA", &Cpu::PHA, &Cpu::IMP, 3, false);

    // PHP
    table[0x08] = make("PHP", &Cpu::PHP, &Cpu::IMP, 3, false);

    // PLA
    table[0x68] = make("PLA", &Cpu::PLA, &Cpu::IMP, 4, false);

    // PLP
    table[0x28] = make("PLP", &Cpu::PLP, &Cpu::IMP, 4, false);

    // ROL
    table[0x2A] = make("ROL", &Cpu::ROL, &Cpu::IMP, 2, false);
    table[0x26] = make("ROL", &Cpu::ROL, &Cpu::ZP0, 5, false);
    table[0x36] = make("ROL", &Cpu::ROL, &Cpu::ZPX, 6, false);
    table[0x2E] = make("ROL", &Cpu::ROL, &Cpu::ABS, 6, false);
    table[0x3E] = make("ROL", &Cpu::ROL, &Cpu::ABSX, 7, false);

    // ROR
    table[0x6A] = make("ROR", &Cpu::ROR, &Cpu::IMP, 2, false);
    table[0x66] = make("ROR", &Cpu::ROR, &Cpu::ZP0, 5, false);
    table[0x76] = make("ROR", &Cpu::ROR, &Cpu::ZPX, 6, false);
    table[0x6E] = make("ROR", &Cpu::ROR, &Cpu::ABS, 6, false);
    table[0x7E] = make("ROR", &Cpu::ROR, &Cpu::ABSX, 7, false);

    // RTI
    table[0x40] = make("RTI", &Cpu::RTI, &Cpu::IMP, 6, false);

    // RTS
    table[0x60] = make("RTS", &Cpu::RTS, &Cpu::IMP, 6, false);

    // SBC
    table[0xE9] = make("SBC", &Cpu::SBC, &Cpu::IMM, 2, false);
    table[0xE5] = make("SBC", &Cpu::SBC, &Cpu::ZP0, 3, false);
    table[0xF5] = make("SBC", &Cpu::SBC, &Cpu::ZPX, 4, false);
    table[0xED] = make("SBC", &Cpu::SBC, &Cpu::ABS, 4, false);
    table[0xFD] = make("SBC", &Cpu::SBC, &Cpu::ABSX, 4, true);
    table[0xF9] = make("SBC", &Cpu::SBC, &Cpu::ABSY, 4, true);
    table[0xE1] = make("SBC", &Cpu::SBC, &Cpu::IDX, 6, false);
    table[0xF1] = make("SBC", &Cpu::SBC, &Cpu::IDY, 5, true);

    // SEC
    table[0x38] = make("SEC", &Cpu::SEC, &Cpu::IMP, 2, false);

    // SED
    table[0xF8] = make("SED", &Cpu::SED, &Cpu::IMP, 2, false);

    // SEI
    table[0x78] = make("SEI", &Cpu::SEI, &Cpu::IMP, 2, false);

    // STA
    table[0x85] = make("STA", &Cpu::STA, &Cpu::ZP0, 3, false);
    table[0x95] = make("STA", &Cpu::STA, &Cpu::ZPX, 4, false);
    table[0x8D] = make("STA", &Cpu::STA, &Cpu::ABS, 4, false);
    table[0x9D] = make("STA", &Cpu::STA, &Cpu::ABSX, 5, false);
    table[0x99] = make("STA", &Cpu::STA, &Cpu::ABSY, 5, false);
    table[0x81] = make("STA", &Cpu::STA, &Cpu::IDX, 6, false);
    table[0x91] = make("STA", &Cpu::STA, &Cpu::IDY, 6, false);

    // STX
    table[0x86] = make("STX", &Cpu::STX, &Cpu::ZP0, 3, false);
    table[0x96] = make("STX", &Cpu::STX, &Cpu::ZPY, 4, false);
    table[0x8E] = make("STX", &Cpu::STX, &Cpu::ABS, 4, false);

    // STY
    table[0x84] = make("STY", &Cpu::STY, &Cpu::ZP0, 3, false);
    table[0x94] = make("STY", &Cpu::STY, &Cpu::ZPX, 4, false);
    table[0x8C] = make("STY", &Cpu::STY, &Cpu::ABS, 4, false);

    // TAX
    table[0xAA] = make("TAX", &Cpu::TAX, &Cpu::IMP, 2, false);

    // TAY
    table[0xA8] = make("TAY", &Cpu::TAY, &Cpu::IMP, 2, false);

    // TSX
    table[0xBA] = make("TSX", &Cpu::TSX, &Cpu::IMP, 2, false);

    // TXA
    table[0x8A] = make("TXA", &Cpu::TXA, &Cpu::IMP, 2, false);

    // TXS
    table[0x9A] = make("TXS", &Cpu::TXS, &Cpu::IMP, 2, false);

    // TYA
    table[0x98] = make("TYA", &Cpu::TYA, &Cpu::IMP, 2, false);

    return table;
}();