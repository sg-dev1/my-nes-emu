#pragma once
#include <cstdint>

class Mapper;

class Ppu {
public:
    Mapper* mapper = nullptr;

    uint8_t registers[8]{};

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);

    void step();
};