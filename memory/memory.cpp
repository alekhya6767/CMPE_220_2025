#include "memory.h"          // Include the memory class definition
#include <iomanip>           // For formatted hex output during memory dump

// ===============================================================
// Constructor: Initialize memory to all zeros
// Also initialize the I/O-mapped registers (OUTPUT & TIMER)
// ===============================================================
Memory::Memory() : mem(MEM_SIZE, 0) {   // Create RAM array of MEM_SIZE bytes
    mem[IO_OUTPUT] = 0;                 // Output port initialized
    mem[IO_TIMER]  = 0;                 // Timer port initialized
}

// ===============================================================
// Read a single byte from memory
// addr → 16-bit memory address
// returns uint8_t at that location
// ===============================================================
uint8_t Memory::read8(uint16_t addr) const {
    return mem[addr];                    // Return 1 byte stored at addr
}

// ===============================================================
// Read a 16-bit word: little-endian format
// low byte at addr, high byte at addr+1
// ===============================================================
uint16_t Memory::read16(uint16_t addr) const {
    uint16_t lo = mem[addr];             // Low byte
    uint16_t hi = mem[addr + 1];         // High byte
    return (hi << 8) | lo;               // Combine to form 16-bit value
}

// ===============================================================
// Write a single byte to memory
// Includes special handling for I/O mapped addresses:
//   - Writing to OUTPUT prints to console
//   - Writing to TIMER updates internal timer
// ===============================================================
void Memory::write8(uint16_t addr, uint8_t value) {

    // ---------------------------
    // OUTPUT PORT (0xFF00)
    // Prints integer value to console
    // ---------------------------
    if (addr == IO_OUTPUT) {
        std::cout << std::dec << (int)value << " " << std::flush;  
        mem[addr] = value;               // Store printed value in memory
        return;                          // Stop further handling
    }

    // ---------------------------
    // TIMER PORT (0xFF01)
    // Simply stores timer value
    // ---------------------------
    if (addr == IO_TIMER) {
        mem[addr] = value;               // Update timer
        return;
    }

    // ---------------------------
    // Regular Memory Write
    // ---------------------------
    mem[addr] = value;                   // Store byte normally
}

// ===============================================================
// Write a 16-bit word using two 8-bit writes
// Uses little-endian encoding
// ===============================================================
void Memory::write16(uint16_t addr, uint16_t value) {
    write8(addr, value & 0xFF);          // Write low byte first
    write8(addr + 1, (value >> 8) & 0xFF);  // Then high byte
}

// ===============================================================
// tick_timer()
// Automatically increments the TIMER register each CPU cycle
// Used to simulate hardware timer behavior
// ===============================================================
void Memory::tick_timer() {
    mem[IO_TIMER]++;                     // Increment timer register
}

// ===============================================================
// dump(start, end)
// Prints memory from start → end (inclusive)
// Used for debugging and CPU HALT dump
// ===============================================================
void Memory::dump(uint16_t start, uint16_t end) const {
    std::cout << "\n--- Memory Dump ---\n";

    for (uint16_t addr = start; addr <= end; addr++) {
        std::cout << "0x"
                  << std::hex                         // Switch to hex output
                  << std::setw(4)                     // 4 hex digits for address
                  << std::setfill('0')                // Pad with zeros
                  << addr
                  << ": 0x"
                  << std::setw(2)                     // 2 hex digits for data
                  << (int)mem[addr]
                  << std::dec                         // Reset formatting to decimal
                  << "\n";
    }
}
