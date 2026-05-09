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
    void ADC(uint16_t addr);
    void AND(uint16_t addr);
    void ASL(uint16_t addr);
    void BCC(uint16_t addr);
    void BCS(uint16_t addr);
    void BEQ(uint16_t addr);
    void BIT(uint16_t addr);
    void BMI(uint16_t addr);
    void BNE(uint16_t addr);
    void BPL(uint16_t addr);
    void BRK(uint16_t addr);
    void BVC(uint16_t addr);
    void BVS(uint16_t addr);
    void CLC(uint16_t addr);
    void CLD(uint16_t addr);
    void CLI(uint16_t addr);
    void CLV(uint16_t addr);
    void CMP(uint16_t addr);
    void CPX(uint16_t addr);
    void CPY(uint16_t addr);
    void DEC(uint16_t addr);
    void DEX(uint16_t addr);
    void DEY(uint16_t addr);
    void EOR(uint16_t addr);
    void INC(uint16_t addr);
    void INX(uint16_t addr);
    void INY(uint16_t addr);
    void JMP(uint16_t addr);
    void JSR(uint16_t addr);
    void LDA(uint16_t addr);
    void LDX(uint16_t addr);
    void LDY(uint16_t addr);
    void LSR(uint16_t addr);
    void NOP(uint16_t addr);
    void ORA(uint16_t addr);
    void PHA(uint16_t addr);
    void PHP(uint16_t addr);
    void PLA(uint16_t addr);
    void PLP(uint16_t addr);
    void ROL(uint16_t addr);
    void ROR(uint16_t addr);
    void RTI(uint16_t addr);
    void RTS(uint16_t addr);
    void SBC(uint16_t addr);
    void SEC(uint16_t addr);
    void SED(uint16_t addr);
    void SEI(uint16_t addr);
    void STA(uint16_t addr);
    void STX(uint16_t addr);
    void STY(uint16_t addr);
    void TAX(uint16_t addr);
    void TAY(uint16_t addr);
    void TSX(uint16_t addr);
    void TXA(uint16_t addr);
    void TXS(uint16_t addr);
    void TYA(uint16_t addr);
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
    // Number of cycles used for instruction timing (instruction level emulator)
    // In future emulation could be done on Micro-op level, so each Cpu::step() executes one micro-op,
    // e.g. decode opcode, fetch low, fetch high, execute, etc.
    uint8_t m_Cycles = 0;

    Bus &m_Bus;

    enum class Flag {
        Carry = 1 << 0, // Carry
        Zero = 1 << 1, // Zero
        InterruptDisable = 1 << 2, // Interrupt Disable
        Decimal = 1 << 3, // Decimal Mode (unused in NES)
        Break = 1 << 4, // Break Command
        Unused = 1 << 5, // Unused
        Overflow = 1 << 6, // Overflow
        Negative = 1 << 7, // Negative
    };

    bool getFlag(Flag flag) const;
    void setFlag(Flag flag, bool value);
    void setZN(uint8_t value) {
        setFlag(Flag::Zero, value == 0);
        setFlag(Flag::Negative, value & 0x80);
    }

    void push(uint8_t val);
    uint8_t pop();
    void push16(uint16_t val);
    uint16_t pop16();
};