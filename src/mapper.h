#pragma once
#include <cstdint>

class Mapper {
public:
    virtual ~Mapper() = default;

    virtual uint8_t readPRG(uint16_t addr) = 0;
    virtual void writePRG(uint16_t addr, uint8_t val) = 0;

    virtual uint8_t readCHR(uint16_t addr) = 0;
};