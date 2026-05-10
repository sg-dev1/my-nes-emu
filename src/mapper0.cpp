#include "mapper0.h"

Mapper0::Mapper0(std::vector<uint8_t> prg, std::vector<uint8_t> chr, const Mirroring mirroring)
    : m_Prg(std::move(prg)), m_Chr(std::move(chr)), m_Mirroring(mirroring) {
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

void Mapper0::writeCHR(uint16_t /*addr*/, uint8_t /*val*/) 
{
    // nop, as CHR ROM is read-only
}

uint16_t Mapper0::mirrorVramAddr(uint16_t addr) const
{
    addr &= 0x0FFF;

    // Table number (0-3)
    uint16_t table  = addr / 0x0400;
    // Offset (address) within the table
    uint16_t offset = addr % 0x0400;

    switch (m_Mirroring)
    {
        case Mirroring::Vertical:
            // NT0 NT1
            // NT0 NT1
            switch (table)
            {
                case 0: return offset;
                case 1: return 0x0400 + offset;
                case 2: return offset;
                case 3: return 0x0400 + offset;
            }
            break;

        case Mirroring::Horizontal:
            // NT0 NT0
            // NT1 NT1
            switch (table)
            {
                case 0: return offset;
                case 1: return offset;
                case 2: return 0x0400 + offset;
                case 3: return 0x0400 + offset;
            }
    }

    return 0;
}
