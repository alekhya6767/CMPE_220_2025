#include "memory.h"
#include <iomanip>

Memory::Memory() : mem(MEM_SIZE, 0) {
    mem[IO_OUTPUT] = 0;
    mem[IO_TIMER]  = 0;
}

uint8_t Memory::read8(uint16_t addr) const {
    return mem[addr];
}

uint16_t Memory::read16(uint16_t addr) const {
    uint16_t lo = mem[addr];
    uint16_t hi = mem[addr + 1];
    return (hi << 8) | lo;
}

void Memory::write8(uint16_t addr, uint8_t value) {

    if (addr == IO_OUTPUT) {
        std::cout << std::dec << (int)value << " " << std::flush;
        mem[addr] = value;
        return;
    }

    if (addr == IO_TIMER) {
        mem[addr] = value;
        return;
    }

    mem[addr] = value;
}

void Memory::write16(uint16_t addr, uint16_t value) {
    write8(addr, value & 0xFF);
    write8(addr + 1, (value >> 8) & 0xFF);
}

void Memory::tick_timer() {
    mem[IO_TIMER]++;     // <-- MUST exist
}

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
