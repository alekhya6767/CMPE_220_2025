#include "memory.h"
#include <iomanip>

// ---------------------------------------------
// Constructor – initialize memory + I/O
// ---------------------------------------------
Memory::Memory() : mem(MEM_SIZE, 0) {
    mem[IO_OUTPUT_NUM]  = 0;
    mem[IO_TIMER]       = 0;
    mem[IO_OUTPUT_CHAR] = 0;
}

// ---------------------------------------------
// Read 8-bit value
// ---------------------------------------------
uint8_t Memory::read8(uint16_t addr) const {
    return mem[addr];
}

// ---------------------------------------------
// Read 16-bit little-endian value
// ---------------------------------------------
uint16_t Memory::read16(uint16_t addr) const {
    uint16_t lo = mem[addr];
    uint16_t hi = mem[addr + 1];
    return (hi << 8) | lo;
}

// ---------------------------------------------
// Write 8-bit value
// ---------------------------------------------
void Memory::write8(uint16_t addr, uint8_t value) {

    // Timer register
    if (addr == IO_TIMER) {
        mem[addr] = value;
        return;
    }

    // Character output (low byte as ASCII)
    if (addr == IO_OUTPUT_CHAR) {
        char c = static_cast<char>(value);
        std::cout << c << std::flush;
        mem[addr] = value;
        return;
    }

    // Default write
    mem[addr] = value;
}

// ---------------------------------------------
// Write 16-bit little-endian value
// ---------------------------------------------
void Memory::write16(uint16_t addr, uint16_t value) {

    // Numeric output port – print decimal number
    if (addr == IO_OUTPUT_NUM) {
        std::cout << std::dec << value << " " << std::flush;
        mem[addr]     = value & 0xFF;
        mem[addr + 1] = (value >> 8) & 0xFF;
        return;
    }

    // Character output port – use low byte as char
    if (addr == IO_OUTPUT_CHAR) {
        char c = static_cast<char>(value & 0xFF);
        std::cout << c << std::flush;
        mem[addr]     = value & 0xFF;
        mem[addr + 1] = (value >> 8) & 0xFF;
        return;
    }

    // Normal RAM write
    write8(addr,     value & 0xFF);
    write8(addr + 1, (value >> 8) & 0xFF);
}

// ---------------------------------------------
// Tick timer – increment timer register
// ---------------------------------------------
void Memory::tick_timer() {
    mem[IO_TIMER]++;     // simple free-running 8-bit timer
}

// ---------------------------------------------
// Dump memory [start, end]
// ---------------------------------------------
void Memory::dump(uint16_t start, uint16_t end) const {
    std::cout << "\n--- Memory Dump ---\n";
    for (uint16_t addr = start; addr <= end; addr++) {
        std::cout << "0x"
                  << std::hex << std::setw(4) << std::setfill('0') << addr
                  << ": 0x"
                  << std::setw(2) << (int)mem[addr]
                  << std::dec << "\n";
    }
}
