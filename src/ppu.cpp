#include "ppu.h"

Ppu::Ppu(Mapper &mapper) : m_Mapper(mapper) {}

uint8_t Ppu::read(uint16_t addr) {
    return m_Registers[addr & 7];
}

void Ppu::write(uint16_t addr, uint8_t val) {
    m_Registers[addr & 7] = val;
}

void Ppu::step() {
    // TODO: rendering
}