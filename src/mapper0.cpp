#include "mapper0.h"

Mapper0::Mapper0(std::vector<uint8_t> prg, std::vector<uint8_t> chr)
    : prg(std::move(prg)), chr(std::move(chr)) {
    is16k = (this->prg.size() == 16384);
}

uint8_t Mapper0::readPRG(uint16_t addr) {
    addr -= 0x8000;

    if (is16k) {
        addr %= 0x4000;
    }

    return prg[addr];
}

void Mapper0::writePRG(uint16_t, uint8_t) {
    // no-op
}

uint8_t Mapper0::readCHR(uint16_t addr) {
    return chr[addr];
}