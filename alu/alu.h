#pragma once   // Prevent multiple includes

#include <cstdint>     // For uint16_t
#include "common.h"    // Import Flags struct

// =========================================
// ALU (Arithmetic Logic Unit)
// Performs all arithmetic and bitwise ops
// =========================================

class ALU {
public:

    // Perform ADD operation
    uint16_t add(uint16_t a, uint16_t b, Flags &flags);

    // Perform SUB operation
    uint16_t sub(uint16_t a, uint16_t b, Flags &flags);

    // Perform bitwise AND
    uint16_t _and(uint16_t a, uint16_t b, Flags &flags);

    // Perform bitwise OR
    uint16_t _or(uint16_t a, uint16_t b, Flags &flags);

    // Perform bitwise XOR
    uint16_t _xor(uint16_t a, uint16_t b, Flags &flags);

    // Compare: sets flags only (ZF, CF)
    void cmp(uint16_t a, uint16_t b, Flags &flags);

    // MOV: simply returns operand B (no flags updated)
    uint16_t mov(uint16_t b);
};
