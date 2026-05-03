#pragma once
#include <cstdint>

class Cpu;
class Ppu;
class Mapper;

class Bus {
public:
    Bus(Ppu & ppu, Mapper &mapper);    

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);

private:
    uint8_t m_ram[2048]{};

    Ppu& m_Ppu;
    Mapper& m_Mapper;
};