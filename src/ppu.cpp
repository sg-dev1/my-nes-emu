#include "ppu.h"

uint8_t Ppu::read(uint16_t addr) {
    return registers[addr & 7];
}

void Ppu::write(uint16_t addr, uint8_t val) {
    registers[addr & 7] = val;
}

void Ppu::step() {
    // TODO: rendering
}