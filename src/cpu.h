#pragma once
#include <cstdint>

class Bus;

class Cpu {
public:
    Cpu(Bus &bus);

    void reset();
    void step();

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);
    uint16_t read16(uint16_t addr);

private:
    uint16_t m_Pc = 0;
    uint8_t m_A = 0, m_X = 0, m_Y = 0, m_Sp = 0, m_Status = 0;

    Bus &m_Bus;
};