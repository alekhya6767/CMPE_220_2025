#include "alu.h"    // Import ALU class and flags
#include <cstdint>  // For uint16_t

// =========================================
// ADD (16-bit)
// Updates: ZF (zero flag), CF (carry flag)
// =========================================
uint16_t ALU::add(uint16_t a, uint16_t b, Flags &flags) {

    // Perform 16-bit addition using 32-bit to detect overflow
    uint32_t result32 = (uint32_t)a + (uint32_t)b;

    // Lower 16 bits are the actual result
    uint16_t result = result32 & 0xFFFF;

    // Carry flag = true if result exceeds 16 bits
    flags.CF = (result32 > 0xFFFF);

    // Zero flag = true if result is exactly 0
    flags.ZF = (result == 0);

    return result;
}

// =========================================
// SUB (16-bit)
// Updates: ZF (zero), CF (borrow/underflow)
// =========================================
uint16_t ALU::sub(uint16_t a, uint16_t b, Flags &flags) {

    // Perform subtraction using 32-bit to detect underflow
    int32_t result32 = (int32_t)a - (int32_t)b;

    // Mask to 16 bits
    uint16_t result = result32 & 0xFFFF;

    // Carry flag = underflow → result < 0
    flags.CF = (result32 < 0);

    // Zero flag
    flags.ZF = (result == 0);

    return result;
}

// =========================================
// Bitwise AND
// Updates: ZF only
// =========================================
uint16_t ALU::_and(uint16_t a, uint16_t b, Flags &flags) {

    uint16_t result = a & b;

    flags.ZF = (result == 0);
    flags.CF = false;    // No carry for AND

    return result;
}

// =========================================
// Bitwise OR
// Updates: ZF only
// =========================================
uint16_t ALU::_or(uint16_t a, uint16_t b, Flags &flags) {

    uint16_t result = a | b;

    flags.ZF = (result == 0);
    flags.CF = false;

    return result;
}

// =========================================
// Bitwise XOR
// Updates: ZF only
// =========================================
uint16_t ALU::_xor(uint16_t a, uint16_t b, Flags &flags) {

    uint16_t result = a ^ b;

    flags.ZF = (result == 0);
    flags.CF = false;

    return result;
}

// =========================================
// CMP (compare)
// Does NOT return value — only sets flags
// =========================================
void ALU::cmp(uint16_t a, uint16_t b, Flags &flags) {

    // Compute internal subtraction to set flags
    int32_t result32 = (int32_t)a - (int32_t)b;

    // Zero flag: true if equal
    flags.ZF = ((a & 0xFFFF) == (b & 0xFFFF));

    // Carry flag: borrow occurred
    flags.CF = (result32 < 0);
}

// =========================================
// MOV (move B into destination)
// No flags affected
// =========================================
uint16_t ALU::mov(uint16_t b) {
    return b;   // pass-through
}
