#include "bus.h"
#include "ppu.h"
#include "mapper.h"

uint8_t Bus::read(uint16_t addr) {
    if (addr <= 0x1FFF) {
        return ram[addr & 0x07FF];
    }
    else if (addr <= 0x3FFF) {
        return ppu->read(0x2000 + (addr & 7));
    }
    else if (addr >= 0x8000) {
        return mapper->readPRG(addr);
    }

    return 0;
}

void Bus::write(uint16_t addr, uint8_t val) {
    if (addr <= 0x1FFF) {
        ram[addr & 0x07FF] = val;
    }
    else if (addr <= 0x3FFF) {
        ppu->write(0x2000 + (addr & 7), val);
    }
    else if (addr >= 0x8000) {
        mapper->writePRG(addr, val);
    }
}