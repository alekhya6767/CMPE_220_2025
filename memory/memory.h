#pragma once   // Prevents this header from being included multiple times

#include <cstdint>      // Provides fixed-size integer types (uint8_t, uint16_t)
#include <vector>       // Used for implementing RAM storage
#include <iostream>     // Needed for I/O-mapped output
#include "common.h"     // Contains memory size constants & I/O addresses

// ===============================================================
// Memory Class
// Implements 64 KB of byte-addressable RAM
// Also handles memory-mapped I/O (OUTPUT and TIMER registers)
// ===============================================================
class Memory {
private:
    // -----------------------------------------------------------
    // mem[]
    // Vector representing CPU RAM (size = MEM_SIZE, from common.h)
    // Each entry is a single byte (uint8_t)
    // -----------------------------------------------------------
    std::vector<uint8_t> mem;

public:

    // -----------------------------------------------------------
    // Constructor
    // Initializes RAM and I/O-mapped registers
    // -----------------------------------------------------------
    Memory();

    // -----------------------------------------------------------
    // Read a single byte from memory
    // addr → 16-bit address (0–65535)
    // Returns uint8_t value stored at that address
    // -----------------------------------------------------------
    uint8_t read8(uint16_t addr) const;

    // -----------------------------------------------------------
    // Read a 16-bit word (little-endian: low byte at addr, high byte at addr+1)
    // -----------------------------------------------------------
    uint16_t read16(uint16_t addr) const;

    // -----------------------------------------------------------
    // Write a single byte to memory
    // Includes special handling for I/O-mapped ports:
    //   - 0xFF00 → OUTPUT port (prints to console)
    //   - 0xFF01 → TIMER register
    // -----------------------------------------------------------
    void write8(uint16_t addr, uint8_t value);

    // -----------------------------------------------------------
    // Write a 16-bit value using two 8-bit writes (little-endian)
    // -----------------------------------------------------------
    void write16(uint16_t addr, uint16_t value);

    // -----------------------------------------------------------
    // tick_timer()
    // Called by CPU once per instruction cycle
    // Increments memory[IO_TIMER]
    // -----------------------------------------------------------
    void tick_timer();

    // -----------------------------------------------------------
    // dump(start, end)
    // Prints memory contents from start → end in hex
    // Used in HALT debug output
    // -----------------------------------------------------------
    void dump(uint16_t start, uint16_t end) const;
};
