#include "ppu.h"
#include "mapper.h"

Ppu::Ppu(Mapper &mapper) : m_Mapper(mapper) {}

uint8_t Ppu::cpuRead(uint16_t addr) {
    switch (addr) {
        case 0x2000: // PPUCTRL
            return m_Ppuctrl;

        case 0x2001: // PPUMASK
            return m_Ppumask;

        case 0x2002: // PPUSTATUS
            return m_Ppustatus;
        
        case 0x2003: // OAMADDR
            return m_Oamaddr;

        case 0x2004: // OAMDATA
            break; // TODO
        case 0x2005: // PPUSCROLL
            break; // TODO
        case 0x2006: // PPUADDR
            break; // TODO

        case 0x2007: // PPUDATA
        {
            uint16_t vram_addr = 0; // TODO: vram address
            return ppuRead(vram_addr);
        }

        default:
            return 0;
    }

    return 0;
}

void Ppu::cpuWrite(uint16_t addr, uint8_t val) {
    switch (addr) {
        case 0x2000: // PPUCTRL
            m_Ppuctrl = val;
            break;

        case 0x2001: // PPUMASK
            m_Ppumask = val;
            break;

        case 0x2002: // PPUSTATUS
            m_Ppustatus = val;
            break;

        case 0x2003: // OAMADDR
            m_Oamaddr = val;
            break;
        
        case 0x2004: // OAMDATA
            break; // TODO
        case 0x2005: // PPUSCROLL
            break; // TODO
        case 0x2006: // PPUADDR
            break; // TODO

        case 0x2007: // PPUDATA
        {
            uint16_t vram_addr = 0; // TODO: vram address
            ppuWrite(vram_addr, val);
        }

        default:
            break;
    }
}

uint8_t Ppu::ppuRead(uint16_t addr)
{
    addr &= 0x3FFF;

    if (addr <= 0x1FFF) {
        return m_Mapper.readCHR(addr);
    }
    // 0x2000-0x2FFF: nametables (mirrored as needed)
    else if (addr <= 0x3EFF) {
        uint16_t mirrored = m_Mapper.mirrorVramAddr(addr);
        return m_NametableRAM[mirrored];
    }
    // 0x3F00-0x3FFF: palette RAM indexes (mirrored every 32 bytes)
    else if (addr <= 0x3FFF) {
        return m_PaletteRAM[mirrorPaletteAddr(addr)];
    }

    return 0;
}

void Ppu::ppuWrite(uint16_t addr, uint8_t data) 
{
    addr &= 0x3FFF;

    if (addr <= 0x1FFF) {
        m_Mapper.writeCHR(addr, data);
    }
    // 0x2000-0x2FFF: nametables (mirrored as needed)
    else if (addr <= 0x3EFF) {
        uint16_t mirrored = m_Mapper.mirrorVramAddr(addr);
        m_NametableRAM[mirrored] = data;
    }
    // 0x3F00-0x3FFF: palette RAM indexes (mirrored every 32 bytes)
    else if (addr <= 0x3FFF) {
        m_PaletteRAM[mirrorPaletteAddr(addr)] = data;
    }
}

uint16_t Ppu::mirrorPaletteAddr(uint16_t addr) const
{
    // Palette RAM is mirrored every 32 bytes
    addr &= 0x1F;

    // The following addresses are mirrored in the PPU hardware
    if (addr == 0x10) addr = 0x00;
    if (addr == 0x14) addr = 0x04;
    if (addr == 0x18) addr = 0x08;
    if (addr == 0x1C) addr = 0x0C;

    return addr;
}

void Ppu::step() {
    // TODO: rendering
}

void Ppu::renderPatternTable(std::array<uint32_t, 128 * 128>& framebuffer, uint16_t patternTableBase) 
{
    // 256 tiles total
    for (int tile = 0; tile < 256; tile++)
    {
        // Tile position in 16x16 grid
        int tileX = tile % 16;
        int tileY = tile / 16;

        // Each tile is 16 bytes
        uint16_t tileAddr = patternTableBase + static_cast<uint16_t>(tile * 16);

        // Each tile are 8x8 pixels, (16*8)/64 = 2 bytes per pixel
        //
        // 8 rows per tile
        for (uint8_t row = 0; row < 8; row++) // row = y
        {
            // Bytes 0-7   : bitplane 0
            uint8_t plane0 = ppuRead(tileAddr + row);
            // Bytes 8-15  : bitplane 1
            uint8_t plane1 = ppuRead(static_cast<uint16_t>(tileAddr + row + 8U));

            // 8 pixels per row
            for (int col = 0; col < 8; col++) // col = x
            {
                uint8_t bit0 = (plane0 >> (7 - col)) & 1;
                uint8_t bit1 = (plane1 >> (7 - col)) & 1;
                uint8_t color = (bit1 << 1) | bit0;
                // For simplicity use grayscale colors: 0 = black, 1 = dark gray, 2 = light gray, 3 = white
                uint32_t rgb = 0;
                switch (color)
                {
                    case 0:
                        rgb = 0xFF000000;
                        break;
                    case 1:
                        rgb = 0xFF555555;
                        break;
                    case 2:
                        rgb = 0xFFAAAAAA;
                        break;
                    case 3:
                        rgb = 0xFFFFFFFF;
                        break;
                }

                int screenX = tileX * 8 + col;
                int screenY =  tileY * 8 + row;
                framebuffer[static_cast<std::size_t>(screenY * 128 + screenX)] = rgb;
            }
        }
    }
}