#include "cartridge.h"
#include "mapper0.h"
#include <fstream>
#include <vector>
#include <iostream>

Cartridge::Cartridge(const std::string& path) {
    std::ifstream file(path, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open ROM\n";
        return;
    }

    uint8_t header[16];
    file.read((char*)header, 16);

    int prgSize = header[4] * 16384;
    int chrSize = header[5] * 8192;

    std::vector<uint8_t> prg(prgSize);
    std::vector<uint8_t> chr(chrSize);

    file.read((char*)prg.data(), prgSize);
    file.read((char*)chr.data(), chrSize);

    // Only Mapper 0 supported
    mapper = std::make_unique<Mapper0>(prg, chr);
}