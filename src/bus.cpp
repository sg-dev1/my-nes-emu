#include "cpu.h"
#include "bus.h"
#include "ppu.h"
#include "mapper.h"

Bus::Bus(Ppu &ppu, Mapper &mapper) : m_Ppu(ppu), m_Mapper(mapper) {}

uint8_t Bus::read(uint16_t addr) {
    if (addr <= 0x1FFF) {
        return m_ram[addr & 0x07FF];
    }
    else if (addr <= 0x3FFF) {
        return m_Ppu.cpuRead(0x2000 + (addr & 7));
    }
    else if (addr >= 0x8000) {
        return m_Mapper.readPRG(addr);
    }

    return 0;
}

void Bus::write(uint16_t addr, uint8_t val) {
    if (addr <= 0x1FFF) {
        m_ram[addr & 0x07FF] = val;
    }
    else if (addr <= 0x3FFF) {
        m_Ppu.cpuWrite(0x2000 + (addr & 7), val);
    }
    else if (addr >= 0x8000) {
        m_Mapper.writePRG(addr, val);
    }
}