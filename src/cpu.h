#pragma once
#include <cstdint>

class Bus;

class Cpu {
public:
    Bus* bus = nullptr;

    uint16_t pc = 0;
    uint8_t a = 0, x = 0, y = 0, sp = 0, status = 0;

    void reset();
    void step();

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);
    uint16_t read16(uint16_t addr);
};