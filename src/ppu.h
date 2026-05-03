#pragma once
#include <cstdint>

class Mapper;

class Ppu {
public:
    Ppu(Mapper &mapper);

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);

    void step();

private:
    Mapper &m_Mapper;
    uint8_t m_Registers[8]{};
};