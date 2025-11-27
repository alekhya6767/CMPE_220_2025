#pragma once     // Prevent multiple includes

#include <cstdint>        // uint16_t, uint8_t
#include "registers.h"    // RegisterFile class
#include "memory.h"       // Memory class
#include "control.h"      // ControlUnit + DecodedInstr
#include "alu.h"          // ALU operations

// ============================================
// CPU CLASS
// Handles:
// - Fetch
// - Decode
// - Execute
// - Timer tick
// - Running programs until HALT
// ============================================
class CPU {
public:
    RegisterFile regs;     // Holds registers + PC + flags
    Memory memory;         // 64 KB memory + I/O
    ControlUnit cu;        // Decodes opcodes into instruction structure
    ALU alu;               // Performs arithmetic/logic ops

    CPU();                 // Constructor

    void load_program(const std::vector<uint8_t>& program, uint16_t start);
    void run();            // Runs program until HALT
    void step();           // Executes ONE instruction
};
