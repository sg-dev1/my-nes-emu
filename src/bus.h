#pragma once
#include <cstdint>

class Cpu;
class Ppu;
class Mapper;

class Bus {
public:
    Cpu* cpu = nullptr;
    Ppu* ppu = nullptr;
    Mapper* mapper = nullptr;

    uint8_t ram[2048]{};

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);
};