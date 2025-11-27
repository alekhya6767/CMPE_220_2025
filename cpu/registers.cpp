#include "registers.h"   // Imports the RegisterFile class definition
#include <iostream>      // Needed for std::cout printing

// ================================
// RegisterFile Constructor
// Initializes all registers to 0
// ================================
RegisterFile::RegisterFile() {
    R.fill(0);   // Set R0–R3 = 0
    PC = 0;      // Program counter starts at address 0
    flags = {};  // Zero out flags (ZF = 0, CF = 0)
}

// ===========================================
// dump()
// Prints registers, PC, and flags to console
// Used for debugging and final CPU state print
// ===========================================
void RegisterFile::dump() const {
    std::cout << "---- Register Dump ----\n";

    // Print each general-purpose register
    for (int i = 0; i < REG_COUNT; i++) {
        std::cout << "R" << i << ": " << R[i] 
                  << " (0x" << std::hex << R[i] << std::dec << ")\n";
    }

    // Print program counter
    std::cout << "PC: " << PC 
              << " (0x" << std::hex << PC << std::dec << ")\n";

    // Print flags
    std::cout << "ZF: " << flags.ZF << "  CF: " << flags.CF << "\n";
}
