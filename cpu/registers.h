#pragma once  // Prevent multiple includes

#include "common.h"    // Import CPU constants + flag struct
#include <array>       // For std::array used to store registers

// =====================================
// Register File
// Holds CPU registers and program counter
// =====================================
class RegisterFile {
public:

    // 4 General Purpose Registers (R0–R3)
    // Each register is 16 bits wide
    std::array<uint16_t, REG_COUNT> R;

    // 16-bit Program Counter (PC)
    uint16_t PC = 0;

    // Status flags (Zero, Carry)
    Flags flags;

    // Constructor – initializes registers & flags
    RegisterFile();

    // Debug: print register and flag values
    void dump() const;
};
