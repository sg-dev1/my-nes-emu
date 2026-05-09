#include "cpu.h"
#include "bus.h"
#include "opcodes.h"

Cpu::Cpu(Bus &bus) : m_Bus(bus) {}

void Cpu::reset() {
    m_Pc = read16(0xFFFC);
    m_A = 0;
    m_X = 0;
    m_Y = 0;
    m_Sp = 0xFD; // start address of stack
    m_Status = 0x24; // IRQ disabled and unused flag set
    m_Cycles = 0;
}

uint8_t Cpu::read(uint16_t addr) {
    return m_Bus.read(addr);
}

void Cpu::write(uint16_t addr, uint8_t val) {
    m_Bus.write(addr, val);
}

uint16_t Cpu::read16(uint16_t addr) {
    uint8_t lo = read(addr);
    uint8_t hi = read(addr + 1);
    return (hi << 8) | lo;
}

void Cpu::push(uint8_t val) {
    write(0x0100 + m_Sp, val);
    m_Sp--;
}

uint8_t Cpu::pop() {
    m_Sp++;
    return read(0x0100 + m_Sp);
}

void Cpu::push16(uint16_t val) {
    push(static_cast<uint8_t>(val >> 8));
    push(static_cast<uint8_t>(val));
}

uint16_t Cpu::pop16() {
    uint8_t lo = pop();
    uint8_t hi = pop();
    return (hi << 8) | lo;
}

void Cpu::step() {
    if (m_Cycles == 0) 
    {
        uint8_t opcode = read(m_Pc++);

        const Opcode& op = OPCODES[opcode];

        AddrResult addrResult = (this->*op.addrmode)();
        (this->*op.instr)(addrResult.addr);
        m_Cycles = static_cast<uint8_t>(op.cycles + (op.extracCyclesOnPageCross && addrResult.pageCrossed ? 1U : 0U));
    }

    m_Cycles--;
}

bool Cpu::getFlag(Flag flag) const
{
    return (m_Status & static_cast<uint8_t>(flag)) != 0;
}

void Cpu::setFlag(Cpu::Flag flag, bool value)
{
    if (value) {
        m_Status |= static_cast<uint8_t>(flag);
    } else {
        m_Status &= ~static_cast<uint8_t>(flag);
    }
}

// ---------------------------------------------------------------------
// Instructions
// ---------------------------------------------------------------------

void Cpu::ADC(uint16_t addr) 
{
    const uint8_t value = read(addr);
    // A = A + memory + C
    uint16_t result = static_cast<uint16_t>(m_A + value + (m_Status & static_cast<uint8_t>(Flag::Carry) ? 1U : 0U));
    m_A = static_cast<uint8_t>(result);
    setZN(m_A);
    setFlag(Flag::Carry, result > 0xFF);
    setFlag(Flag::Overflow, ((result ^ m_A) & (result ^ value) & 0x80) != 0);
}

void Cpu::AND(uint16_t addr) 
{
    m_A &= read(addr);
    setZN(m_A);
}

void Cpu::ASL(uint16_t addr) 
{
    uint8_t val = read(addr);
    setFlag(Flag::Carry, val & 0x80);
    val <<= 1;
    write(addr, val);
    setZN(val);
}

void Cpu::BCC(uint16_t addr)
{
    if (!getFlag(Flag::Carry)) {
        int offset = static_cast<int8_t>(read(addr));
        m_Pc = static_cast<uint16_t>(m_Pc + 2 + offset);
        // Requires 3 cycles if branch taken, 4 if page crossed (and branch is taken)
        //   this means the page crossed penalty is only relevant if the branch is taken.
        // TODO should return a bool etc. when branch is taken
    }
}

void Cpu::BCS(uint16_t addr)
{
    if (getFlag(Flag::Carry)) {
        int offset = static_cast<int8_t>(read(addr));
        m_Pc = static_cast<uint16_t>(m_Pc + 2 + offset);
        // TODO same considerations as in BCC
    }
}

void Cpu::BEQ(uint16_t addr)
{
    if (getFlag(Flag::Zero)) {
        int offset = static_cast<int8_t>(read(addr));
        m_Pc = static_cast<uint16_t>(m_Pc + 2 + offset);
        // TODO same considerations as in BCC
    }
}

void Cpu::BIT(uint16_t addr)
{
    uint8_t value = read(addr);
    setFlag(Flag::Overflow, (value & 0x40) != 0);
    setZN(value);
}

void Cpu::BMI(uint16_t addr)
{
    if (getFlag(Flag::Negative)) {
        int offset = static_cast<int8_t>(read(addr));
        m_Pc = static_cast<uint16_t>(m_Pc + 2 + offset);
        // TODO same considerations as in BCC
    }
}

void Cpu::BNE(uint16_t addr)
{
    if (!getFlag(Flag::Zero)) {
        int offset = static_cast<int8_t>(read(addr));
        m_Pc = static_cast<uint16_t>(m_Pc + 2 + offset);
        // TODO same considerations as in BCC
    }
}

void Cpu::BPL(uint16_t addr)
{
    if (!getFlag(Flag::Negative)) {
        int offset = static_cast<int8_t>(read(addr));
        m_Pc = static_cast<uint16_t>(m_Pc + 2 + offset);
        // TODO same considerations as in BCC
    }
}

void Cpu::BRK(uint16_t /*addr*/)
{
    m_Pc++; // skip one byte
    push16(m_Pc);
    setFlag(Flag::Break, true);
    push(m_Status);
    setFlag(Flag::InterruptDisable, true);
    m_Pc = read16(0xFFFE);
}

void Cpu::BVC(uint16_t addr)
{
    if (!getFlag(Flag::Overflow)) {
        int offset = static_cast<int8_t>(read(addr));
        m_Pc = static_cast<uint16_t>(m_Pc + 2 + offset);
        // TODO same considerations as in BCC
    }
}

void Cpu::BVS(uint16_t addr)
{
    if (getFlag(Flag::Overflow)) {
        int offset = static_cast<int8_t>(read(addr));
        m_Pc = static_cast<uint16_t>(m_Pc + 2 + offset);
        // TODO same considerations as in BCC
    }
}

void Cpu::CLC(uint16_t /*addr*/)
{
    setFlag(Flag::Carry, false);
}

void Cpu::CLD(uint16_t /*addr*/)
{
    setFlag(Flag::Decimal, false);
}

void Cpu::CLI(uint16_t /*addr*/)
{
    setFlag(Flag::InterruptDisable, false);
}

void Cpu::CLV(uint16_t /*addr*/)
{
    setFlag(Flag::Overflow, false);
}

void Cpu::CMP(uint16_t addr)
{
    uint8_t value = read(addr);
    uint16_t result = static_cast<uint16_t>(m_A - value);
    setFlag(Flag::Carry, result < 0x100);
    setZN(static_cast<uint8_t>(result));
}

void Cpu::CPX(uint16_t addr)
{
    uint8_t value = read(addr);
    uint16_t result = static_cast<uint16_t>(m_X - value);
    setFlag(Flag::Carry, result < 0x100);
    setZN(static_cast<uint8_t>(result));
}

void Cpu::CPY(uint16_t addr)
{
    uint8_t value = read(addr);
    uint16_t result = static_cast<uint16_t>(m_Y - value);
    setFlag(Flag::Carry, result < 0x100);
    setZN(static_cast<uint8_t>(result));
}

void Cpu::DEC(uint16_t addr)
{
    uint8_t val = read(addr);
    val--;
    write(addr, val);
    setZN(val);
}

void Cpu::DEX(uint16_t /*addr*/)
{
    m_X--;
    setZN(m_X);
}

void Cpu::DEY(uint16_t /*addr*/)
{
    m_Y--;
    setZN(m_Y);
}

void Cpu::EOR(uint16_t addr)
{
    m_A ^= read(addr);
    setZN(m_A);
}

void Cpu::INC(uint16_t addr)
{
    uint8_t val = read(addr);
    val++;
    write(addr, val);
    setZN(val);
}

void Cpu::INX(uint16_t /*addr*/)
{
    m_X++;
    setZN(m_X);
}

void Cpu::INY(uint16_t /*addr*/)
{
    m_Y++;
    setZN(m_Y);
}

void Cpu::JMP(uint16_t addr) 
{
    m_Pc = addr;
}

void Cpu::JSR(uint16_t addr)
{
    push16(m_Pc - 1); // push return address - 1
    m_Pc = addr;
}

void Cpu::LDA(uint16_t addr) 
{
    m_A = read(addr);
    setZN(m_A);
}

void Cpu::LDX(uint16_t addr) 
{
    m_X = read(addr);
    setZN(m_X);
}

void Cpu::LDY(uint16_t addr) 
{
    m_Y = read(addr);
    setZN(m_Y);
}

void Cpu::LSR(uint16_t addr)
{
    uint8_t val = read(addr);
    setFlag(Cpu::Flag::Carry, val & 0x1);
    val >>= 1;
    write(addr, val);
    setZN(val);
}

void Cpu::NOP(uint16_t /*addr*/)
{
    // nop
}

void Cpu::ORA(uint16_t addr)
{
    m_A |= read(addr);
    setZN(m_A);
}

void Cpu::PHA(uint16_t /*addr*/)
{
    push(m_A);
}

void Cpu::PHP(uint16_t /*addr*/)
{
    push(m_Status | static_cast<uint8_t>(Flag::Break) | static_cast<uint8_t>(Flag::Unused));
}

void Cpu::PLA(uint16_t /*addr*/)
{
    m_A = pop();
    setZN(m_A);
}

void Cpu::PLP(uint16_t /*addr*/) 
{
    m_Status = pop();
    setFlag(Flag::Break, false);
    setFlag(Flag::Unused, true);
}

void Cpu::ROL(uint16_t addr) 
{
    uint8_t val = read(addr);
    bool newCarry = (val & 0x80) != 0;
    val <<= 1;
    if (getFlag(Flag::Carry)) {
        val |= 0x01;
    }
    write(addr, val);
    setZN(val);
    setFlag(Flag::Carry, newCarry);
}

void Cpu::ROR(uint16_t addr) 
{
    uint8_t val = read(addr);
    bool newCarry = (val & 0x01) != 0;
    val >>= 1;
    if (getFlag(Flag::Carry)) {
        val |= 0x80;
    }
    write(addr, val);
    setZN(val);
    setFlag(Flag::Carry, newCarry);
}

void Cpu::RTI(uint16_t /*addr*/) 
{
    m_Status = pop();
    setFlag(Flag::Break, false);
    setFlag(Flag::Unused, true);
    m_Pc = pop16();
}

void Cpu::RTS(uint16_t /*addr*/) 
{
    m_Pc = pop16();
    m_Pc++; // increment to point to the next instruction after the JSR
}

void Cpu::SBC(uint16_t addr) 
{
    const uint8_t value = read(addr);
    // A = A - memory - (1 - C)
    uint16_t result = static_cast<uint16_t>(m_A - value - (getFlag(Flag::Carry) ? 0U : 1U));
    m_A = static_cast<uint8_t>(result);
    setZN(m_A);
    setFlag(Flag::Carry, result < 0x100);
    setFlag(Flag::Overflow, ((result ^ m_A) & (result ^ ~value) & 0x80) != 0);
}

void Cpu::SEC(uint16_t /*addr*/) 
{
    setFlag(Flag::Carry, true);
}

void Cpu::SED(uint16_t /*addr*/) 
{
    setFlag(Flag::Decimal, true);
}

void Cpu::SEI(uint16_t /*addr*/) 
{
    setFlag(Flag::InterruptDisable, true);
}

void Cpu::STA(uint16_t addr) 
{
    write(addr, m_A);
}

void Cpu::STX(uint16_t addr)
{
    write(addr, m_X);
}

void Cpu::STY(uint16_t addr)
{
    write(addr, m_Y);
}

void Cpu::TAX(uint16_t /*addr*/)
{
    m_X = m_A;
    setZN(m_X);
}

void Cpu::TAY(uint16_t /*addr*/)
{
    m_Y = m_A;
    setZN(m_Y);
}

void Cpu::TSX(uint16_t /*addr*/)
{
    m_X = m_Sp;
    setZN(m_X);
}

void Cpu::TXA(uint16_t /*addr*/)
{
    m_A = m_X;
    setZN(m_A);
}

void Cpu::TXS(uint16_t /*addr*/)
{
    m_Sp = m_X;
}

void Cpu::TYA(uint16_t /*addr*/)
{
    m_A = m_Y;
    setZN(m_A);
}

void Cpu::XXX(uint16_t /*addr*/) 
{
    // illegal opcode
}

// ---------------------------------------------------------------------
// Addressing modes
// ---------------------------------------------------------------------

// immediate - value is the next byte
AddrResult Cpu::IMM() 
{
    return {m_Pc++, false};
}

// absolute - full 16-bit address
AddrResult Cpu::ABS() 
{
    uint16_t addr = read16(m_Pc);
    m_Pc += 2;
    return {addr, false};
}

AddrResult Cpu::ABSX() 
{
    uint16_t base = read16(m_Pc);
    uint16_t addr = base + m_X;
    m_Pc += 2;
    bool pageCrossed = (base & 0xFF00) != (addr & 0xFF00);
    return {addr, pageCrossed};
}

AddrResult Cpu::ABSY() 
{
    uint16_t base = read16(m_Pc);
    uint16_t addr = base + m_Y;
    m_Pc += 2;
    bool pageCrossed = (base & 0xFF00) != (addr & 0xFF00);
    return {addr, pageCrossed};
}

AddrResult Cpu::ZP0() 
{
    uint8_t addr = read(m_Pc++);
    return {addr, false};
}

AddrResult Cpu::ZPX() 
{
    uint8_t base = read(m_Pc++);
    uint8_t addr = base + m_X;
    return {addr, false};
}

AddrResult Cpu::ZPY() 
{
    uint8_t base = read(m_Pc++);
    uint8_t addr = base + m_Y;
    return {addr, false};
}

AddrResult Cpu::REL() 
{
    int offset = static_cast<int8_t>(read(m_Pc++));
    uint16_t addr = static_cast<uint16_t>(m_Pc + offset);
    bool pageCrossed = (m_Pc & 0xFF00) != (addr & 0xFF00);
    return {addr, pageCrossed};
}

AddrResult Cpu::IND() 
{
    uint16_t ptr = read16(m_Pc);
    m_Pc += 2;
    // 6502 bug: if the low byte is 0xFF, it does not cross to the next page
    uint16_t addr;
    if ((ptr & 0x00FF) == 0x00FF) {
        uint8_t lo = read(ptr);
        uint8_t hi = read(ptr & 0xFF00);
        addr = (hi << 8) | lo;
    } else {
        addr = read16(ptr);
    }
    return {addr, false};
}

// (addr,X)
AddrResult Cpu::IDX() 
{
    uint8_t base = read(m_Pc++);
    uint8_t ptr = base + m_X;
    uint16_t addr = read16(ptr);
    return {addr, false};
}

// (addr),Y
AddrResult Cpu::IDY() 
{
    uint8_t ptr = read(m_Pc++);
    uint16_t base = read16(ptr);
    uint16_t addr = base + m_Y;
    bool pageCrossed = (base & 0xFF00) != (addr & 0xFF00);
    return {addr, pageCrossed};
}

AddrResult Cpu::IMP() 
{
    m_Pc++; // skip opcode
    return {0  /*invalid address, at m_Pc there is the next instruction*/, false};
}
