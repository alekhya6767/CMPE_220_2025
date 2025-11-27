#pragma once   // Prevent multiple includes

#include <cstdint>      // For uint8_t, uint16_t
#include "common.h"     // Flags + constants

// =========================================
// CONTROL UNIT
// Decodes instruction opcodes and determines:
// - instruction type
// - which ALU operation to use
// - which registers to read/write
// - whether instruction is IMM / REG / MEM / JMP
// =========================================

// Enum describing ALU operations
enum class ALUOp {
    NONE,     // No ALU operation
    MOV,
    ADD,
    SUB,
    AND_,
    OR_,
    XOR_,
    CMP
};

// Enum describing instruction category
enum class InstrType {
    NONE,
    REG_IMM,       // MOV Rn, imm
    REG_REG,       // MOV Rn, Rm
    ALU_REG_REG,   // ADD Rn, Rm
    LOAD_WORD,
    STORE_WORD,
    JUMP,
    JUMP_COND,
    HALT
};

// Structure holding decoded instruction info
struct DecodedInstr {
    InstrType type;   // Type of instruction
    ALUOp alu_op;     // ALU operation to perform (if any)
    uint8_t rd;       // Destination register
    uint8_t rs;       // Source register
    uint16_t imm;     // Immediate value or memory address
};

class ControlUnit {
public:
    // Decode a raw opcode + operands into a structured instruction
    DecodedInstr decode(uint8_t opcode, uint16_t operand1, uint16_t operand2);
};
