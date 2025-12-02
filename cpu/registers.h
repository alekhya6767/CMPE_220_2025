#pragma once
#include <cstdint>
#include "common.h"

// =======================================
// Register File for Software CPU
// General purpose registers: R0–R3
// Program Counter (PC)
// Stack Pointer (SP)
// Flags register (ZF, CF)
// =======================================

class RegisterFile {
public:
    uint16_t R[REG_COUNT];  // R0–R3
    uint16_t PC;            // Program Counter
    uint16_t SP;            // Stack Pointer
    Flags flags;            // Flags (ZF, CF)

    RegisterFile();
    void dump() const;
};
