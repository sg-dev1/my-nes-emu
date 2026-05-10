#pragma once
#include <cstdint>

class Mapper;

class Ppu {
public:
    Ppu(Mapper &mapper);

    uint8_t cpuRead(uint16_t addr);
    void cpuWrite(uint16_t addr, uint8_t val);

    void step();

private:
    uint8_t ppuRead(uint16_t addr);
    void ppuWrite(uint16_t addr, uint8_t data);

    uint16_t mirrorPaletteAddr(uint16_t addr) const;

private:
    Mapper &m_Mapper;

    // registers
    uint8_t m_Ppuctrl;
    uint8_t m_Ppumask;
    uint8_t m_Ppustatus;
    uint8_t m_Oamaddr;

    uint8_t m_NametableRAM[2048]{}; // vram
    uint8_t m_PaletteRAM[32]{};
};