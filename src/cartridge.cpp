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

    // iNes File format: https://www.nesdev.org/wiki/INES
    // 1. 16 bytes header
    // 2. Trainer if present (0 or 512 bytes)
    // 3. PRG ROM data
    // 4. CHR ROM data
    //
    // Header:
    // 0-3: "NES" followed by MS-DOS end-of-file
    // 4: Size of PRG ROM in 16 KB units
    // 5: Size of CHR ROM in 8 KB units (0 means the board uses CHR RAM)
    // 6: Flags 6
    // 7: Flags 7
    // 8: Size of PRG RAM (rarely used)
    // 9-15: usually zero, ignore them
    uint8_t header[16];
    file.read((char*)header, 16);

    unsigned prgSize = header[4] * 16384U;
    unsigned chrSize = header[5] * 8192U;
    uint8_t flags6 = header[6];
    uint8_t flags7 = header[7];

    uint8_t mapperNum = (flags7 & 0xF0) | (flags6 >> 4);
    Mirroring mirroring = (flags6 & 0x01) ? Mirroring::Vertical : Mirroring::Horizontal;

    // Skip trainer if present (rarely used by games)
    bool has_trainer = (flags6 & 0x04) != 0;
    if (has_trainer) {
        file.seekg(512, std::ios::cur);
    }

    std::vector<uint8_t> prg(prgSize);
    std::vector<uint8_t> chr(chrSize);

    file.read((char*)prg.data(), prgSize);
    file.read((char*)chr.data(), chrSize);

    if (mapperNum != 0) {
        std::cerr << "Unsupported mapper: " << (int)mapperNum << "\n";
        return;
    }
    // Only Mapper 0 supported
    m_pMapper = std::make_unique<Mapper0>(prg, chr, mirroring);
}

Mapper& Cartridge::getMapper() noexcept
{
    return *m_pMapper;
}