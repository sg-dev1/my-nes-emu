#include "mapper0.h"

Mapper0::Mapper0(std::vector<uint8_t> prg, std::vector<uint8_t> chr)
    : m_Prg(std::move(prg)), m_Chr(std::move(chr)) {
    m_Is16k = (this->m_Prg.size() == 16384);
}

uint8_t Mapper0::readPRG(uint16_t addr) {
    addr -= 0x8000;

    if (m_Is16k) {
        addr %= 0x4000;
    }

    return m_Prg[addr];
}

void Mapper0::writePRG(uint16_t, uint8_t) {
    // no-op
}

uint8_t Mapper0::readCHR(uint16_t addr) {
    return m_Chr[addr];
}