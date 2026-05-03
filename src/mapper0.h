#pragma once
#include "mapper.h"
#include <vector>

class Mapper0 : public Mapper {
public:
    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;

    bool is16k;

    Mapper0(std::vector<uint8_t> prg, std::vector<uint8_t> chr);

    uint8_t readPRG(uint16_t addr) override;
    void writePRG(uint16_t addr, uint8_t val) override;
    uint8_t readCHR(uint16_t addr) override;
};