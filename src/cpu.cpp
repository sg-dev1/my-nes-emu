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

void Cpu::step() {
    uint8_t opcode = read(m_Pc++);

    const Opcode& op = OPCODES[opcode];

    AddrResult addrResult = (this->*op.addrmode)();
    (this->*op.instr)(addrResult.addr);
    m_Cycles += op.cycles + (op.extracCyclesOnPageCross && addrResult.pageCrossed ? 1 : 0);
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

void Cpu::STA(uint16_t addr) 
{
    write(addr, m_A);
}

void Cpu::XXX(uint16_t addr) 
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
    int8_t offset = read(m_Pc++);
    uint16_t addr = m_Pc + offset;
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
    return {0, false};
}
