#include "cpu.h"
#include "bus.h"

Cpu::Cpu(Bus &bus) : m_Bus(bus) {}

void Cpu::reset() {
    m_Pc = read16(0xFFFC);
}

uint8_t Cpu::read(uint16_t addr) {
    return m_Bus.read(addr);
}

void Cpu::write(uint16_t addr, uint8_t val) {
    m_Bus.write(addr, val);
}

uint16_t Cpu::read16(uint16_t addr) {
    uint8_t lo = read(addr);
    uint8_t hi = read(addr + 1);
    return (hi << 8) | lo;
}

void Cpu::step() {
    uint8_t opcode = read(m_Pc++);

    switch (opcode) {
        case 0xA9: { // LDA immediate
            uint8_t val = read(m_Pc++);
            m_A = val;
            break;
        }
        default:
            // TODO: more opcodes
            break;
    }
}