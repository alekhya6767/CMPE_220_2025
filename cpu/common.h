#pragma once

#include <cstdint>

// ================================================================
// CPU CONSTANTS
// ================================================================

// 64 KB memory
static const int MEM_SIZE = 65536;

// Total register count NOW includes SP (R4) and BP (R5)
static const int REG_COUNT = 6;

// Memory-mapped I/O
// I/O mapped addresses
static const uint16_t IO_OUTPUT_NUM  = 0xFF00;  // print integer numbers
static const uint16_t IO_TIMER       = 0xFF01;  // timer
static const uint16_t IO_OUTPUT_CHAR = 0xFF10;  // print ASCII characters

// ================================================================
// CPU FLAGS
// ================================================================
struct Flags {
    bool ZF = false;   // Zero Flag
    bool CF = false;   // Carry Flag
};

// ================================================================
// INSTRUCTION OPCODES
// ================================================================

// MOV & arithmetic
static const uint8_t OP_MOVI = 0x10;
static const uint8_t OP_MOV  = 0x11;

static const uint8_t OP_ADD  = 0x20;
static const uint8_t OP_SUB  = 0x21;
static const uint8_t OP_AND  = 0x22;
static const uint8_t OP_OR   = 0x23;
static const uint8_t OP_XOR  = 0x24;
static const uint8_t OP_CMP  = 0x25;

// Load/store
static const uint8_t OP_LOAD  = 0x30;
static const uint8_t OP_STORE = 0x31;

// Jumps
static const uint8_t OP_JMP = 0x40;
static const uint8_t OP_JZ  = 0x41;
static const uint8_t OP_JNZ = 0x42;

// NEW: Stack + function calls
static const uint8_t OP_PUSH = 0x50;
static const uint8_t OP_POP  = 0x51;
static const uint8_t OP_CALL = 0x52;
static const uint8_t OP_RET  = 0x53;

// HALT
static const uint8_t OP_HALT = 0xFF;

// ================================================================
// ALU Operations
// ================================================================
enum class ALUOp {
    NONE,
    MOV,
    ADD,
    SUB,
    AND_,
    OR_,
    XOR_,
    CMP
};

// ================================================================
// Instruction Types for CPU
// ================================================================
enum class InstrType {
    NONE,
    REG_IMM,         // MOVI
    REG_REG,         // MOV
    ALU_REG_REG,     // ADD, SUB...
    LOAD_WORD,       // LOAD
    STORE_WORD,      // STORE
    JUMP,            // JMP
    JUMP_COND,       // JZ, JNZ
    PUSH_REG,        // PUSH
    POP_REG,         // POP
    CALL,            // CALL
    RET,             // RET
    HALT             // HALT
};

// ================================================================
// Decoded Instruction Structure
// ================================================================
struct DecodedInstr {
    InstrType type = InstrType::NONE;

    uint16_t rd = 0;   // destination register
    uint16_t rs = 0;   // source register
    uint16_t imm = 0;  // immediate or address

    ALUOp alu_op = ALUOp::NONE;  // ALU operation
};
