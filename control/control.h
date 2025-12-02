#pragma once

#include <cstdint>
#include "common.h"

// ========================================================================
// ControlUnit
// Decodes raw opcode + operands into a structured DecodedInstr
// The CPU fetches:
//   opcode (1 byte)
//   op1    (16-bit)
//   op2    (16-bit)
//
// The control unit determines:
//   - instruction type (MOV, ALU, JUMP, CALL, etc.)
//   - target registers or immediate values
//   - ALU operation type
// ========================================================================
class ControlUnit {
public:
    // Decode instruction into DecodedInstr structure
    DecodedInstr decode(uint8_t opcode, uint16_t op1, uint16_t op2);
};
