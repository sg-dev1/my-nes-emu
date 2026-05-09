#pragma once
#include <cstdint>

class Bus;
class AddrResult;

class Cpu {
public:
    Cpu(Bus &bus);

    void reset();
    void step();

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);
    uint16_t read16(uint16_t addr);

    // Instructions
    void LDA(uint16_t addr);
    void LDX(uint16_t addr);
    void LDY(uint16_t addr);
    void STA(uint16_t addr);
    void XXX(uint16_t addr); // illegal/unimplemented

    // Addressing modes
    AddrResult IMM();
    AddrResult ABS();
    AddrResult ABSX();
    AddrResult ABSY();
    AddrResult ZP0();
    AddrResult ZPX();
    AddrResult ZPY();
    AddrResult REL();
    AddrResult IND();
    AddrResult IDX();  // (addr,X)
    AddrResult IDY();  // (addr),Y
    AddrResult IMP();

private:
    uint16_t m_Pc = 0;
    uint8_t m_A = 0, m_X = 0, m_Y = 0, m_Sp = 0xFD, m_Status = 0x24;
    // Number of cycles cpu has performed since reset
    uint64_t m_Cycles = 0;

    Bus &m_Bus;

    enum class Flag {
        C = 1 << 0, // Carry
        Z = 1 << 1, // Zero
        I = 1 << 2, // Interrupt Disable
        D = 1 << 3, // Decimal Mode (unused in NES)
        B = 1 << 4, // Break Command
        U = 1 << 5, // Unused
        V = 1 << 6, // Overflow
        N = 1 << 7, // Negative
    };

    void setFlag(Flag flag, bool value);
    void setZN(uint8_t value) {
        setFlag(Flag::Z, value == 0);
        setFlag(Flag::N, value & 0x80);
    }
};