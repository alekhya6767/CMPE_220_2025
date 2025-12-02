#include "registers.h"
#include <iostream>
#include <iomanip>

// =======================================
// Constructor: initialize all registers
// =======================================
RegisterFile::RegisterFile() {
    for (int i = 0; i < REG_COUNT; i++)
        R[i] = 0;

    PC = 0;

    // Stack grows downward → start at top
    SP = 0xFFFE;

    flags.ZF = false;
    flags.CF = false;
}

// =======================================
// Dump register state (debug/diagnostic)
// =======================================
void RegisterFile::dump() const {
    std::cout << "---- Register Dump ----\n";
    for (int i = 0; i < REG_COUNT; i++) {
        std::cout << "R" << i << ": "
                  << R[i] << " (0x"
                  << std::hex << std::setw(4) << std::setfill('0') << R[i]
                  << std::dec << ")\n";
    }

    std::cout << "PC: " << PC << " (0x"
              << std::hex << PC << std::dec << ")\n";

    std::cout << "SP: " << SP << " (0x"
              << std::hex << SP << std::dec << ")\n";

    std::cout << "ZF: " << flags.ZF
              << "  CF: " << flags.CF
              << "\n\n";
}
