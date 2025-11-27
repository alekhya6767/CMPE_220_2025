#pragma once   // Prevents this file from being included multiple times

#include <cstdint>   // Provides fixed-size integer types like uint16_t

// ===============================
// CPU CONFIGURATION CONSTANTS
// ===============================

// Total number of general-purpose registers (R0–R3 → 4 registers)
static const int REG_COUNT = 8;

// Total memory size for the CPU (64 KB = 65536 bytes)
static const int MEM_SIZE = 65536;

// ===============================
// MEMORY-MAPPED I/O LOCATIONS
// ===============================

// Address where CPU writes characters for output
static const uint16_t IO_OUTPUT = 0xFF00;

// Address used as a simple timer (CPU increments this value automatically)
static const uint16_t IO_TIMER  = 0xFF01;

// ===============================
// CPU FLAG REGISTER (STATUS FLAGS)
// ===============================
struct Flags {
    bool ZF = false;   // Zero Flag: set if ALU result is zero
    bool CF = false;   // Carry Flag: set if arithmetic operation overflows 16 bits
};
