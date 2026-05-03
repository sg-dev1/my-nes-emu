#include "bus.h"
#include "cpu.h"
#include "ppu.h"
#include "cartridge.h"

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: nes_emulator <rom>\n";
        return 1;
    }

    Cartridge cart(argv[1]);
    Ppu ppu(cart.getMapper());
    Bus bus(ppu, cart.getMapper());
    Cpu cpu(bus);

    cpu.reset();

    while (true) {
        cpu.step();

        ppu.step();
        ppu.step();
        ppu.step();
    }

    return 0;
}