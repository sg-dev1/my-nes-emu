#include "cpu.h"
#include "bus.h"

void Cpu::reset() {
    pc = read16(0xFFFC);
}

uint8_t Cpu::read(uint16_t addr) {
    return bus->read(addr);
}

void Cpu::write(uint16_t addr, uint8_t val) {
    bus->write(addr, val);
}

uint16_t Cpu::read16(uint16_t addr) {
    uint8_t lo = read(addr);
    uint8_t hi = read(addr + 1);
    return (hi << 8) | lo;
}

void Cpu::step() {
    uint8_t opcode = read(pc++);

    switch (opcode) {
        case 0xA9: { // LDA immediate
            uint8_t val = read(pc++);
            a = val;
            break;
        }
        default:
            // TODO: more opcodes
            break;
    }
}