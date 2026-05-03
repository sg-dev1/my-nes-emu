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

    Bus bus;
    Cpu cpu;
    Ppu ppu;

    bus.cpu = &cpu;
    bus.ppu = &ppu;
    bus.mapper = cart.mapper.get();

    cpu.bus = &bus;
    ppu.mapper = cart.mapper.get();

    cpu.reset();

    while (true) {
        cpu.step();

        for (int i = 0; i < 3; i++) {
            ppu.step();
        }
    }

    return 0;
}