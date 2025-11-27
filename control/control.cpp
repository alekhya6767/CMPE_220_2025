#include "control.h"

// =====================================================
// SAFETY HELPER: Clamp register index to valid range
// =====================================================
static inline uint8_t clamp_reg(uint16_t value) {
    // Prevent invalid register access (R4, R5, etc.)
    // REG_COUNT is defined in common.h
    if (value >= REG_COUNT)
        return value % REG_COUNT;   // wrap to usable register
    return value;
}

// =====================================================
// decode()
// Takes an opcode + operands and produces a DecodedInstr.
// =====================================================
DecodedInstr ControlUnit::decode(uint8_t opcode, uint16_t op1, uint16_t op2) {

    DecodedInstr d{};
    d.type = InstrType::NONE;
    d.alu_op = ALUOp::NONE;

    // =====================================================
    // MOVI Rn, imm   (Opcode: 0x10)
    // =====================================================
    if (opcode == 0x10) {
        d.type = InstrType::REG_IMM;
        d.alu_op = ALUOp::MOV;

        d.rd  = clamp_reg(op1);  // FIX: make sure register index is valid
        d.imm = op2;

        return d;
    }

    // =====================================================
    // MOV Rn, Rm   (Opcode: 0x11)
    // =====================================================
    if (opcode == 0x11) {
        d.type = InstrType::REG_REG;
        d.alu_op = ALUOp::MOV;

        d.rd = clamp_reg(op1);
        d.rs = clamp_reg(op2);

        return d;
    }

    // =====================================================
    // ADD Rn, Rm   (Opcode: 0x20)
    // =====================================================
    if (opcode == 0x20) {
        d.type = InstrType::ALU_REG_REG;
        d.alu_op = ALUOp::ADD;

        d.rd = clamp_reg(op1);
        d.rs = clamp_reg(op2);

        return d;
    }

    // =====================================================
    // SUB Rn, Rm   (Opcode: 0x21)
    // =====================================================
    if (opcode == 0x21) {
        d.type = InstrType::ALU_REG_REG;
        d.alu_op = ALUOp::SUB;

        d.rd = clamp_reg(op1);
        d.rs = clamp_reg(op2);

        return d;
    }

    // =====================================================
    // AND / OR / XOR / CMP (0x22–0x25)
    // =====================================================
    if (opcode == 0x22 || opcode == 0x23 || opcode == 0x24 || opcode == 0x25) {
        d.type = InstrType::ALU_REG_REG;

        if (opcode == 0x22) d.alu_op = ALUOp::AND_;
        if (opcode == 0x23) d.alu_op = ALUOp::OR_;
        if (opcode == 0x24) d.alu_op = ALUOp::XOR_;
        if (opcode == 0x25) d.alu_op = ALUOp::CMP;

        d.rd = clamp_reg(op1);
        d.rs = clamp_reg(op2);

        return d;
    }

    // =====================================================
    // LOAD Rn, [addr]  (Opcode: 0x30)
    // =====================================================
    if (opcode == 0x30) {
        d.type = InstrType::LOAD_WORD;
        d.rd   = clamp_reg(op1);
        d.imm  = op2;
        return d;
    }

    // =====================================================
    // STORE Rn, [addr] (Opcode: 0x31)
    // =====================================================
    if (opcode == 0x31) {
        d.type = InstrType::STORE_WORD;
        d.rs   = clamp_reg(op1);
        d.imm  = op2;
        return d;
    }

    // =====================================================
    // JMP addr (Opcode: 0x40)
    // =====================================================
    if (opcode == 0x40) {
        d.type = InstrType::JUMP;
        d.imm  = op1;     // jump target
        return d;
    }

    // =====================================================
    // JZ addr (Opcode: 0x41)
    // =====================================================
    if (opcode == 0x41) {
        d.type = InstrType::JUMP_COND;
        d.imm  = op1;
        return d;
    }

    // =====================================================
    // JNZ addr (Opcode: 0x42)
    // =====================================================
    if (opcode == 0x42) {
        d.type = InstrType::JUMP_COND;
        d.imm  = op1;
        return d;
    }

    // =====================================================
    // HALT (Opcode: 0xFF)
    // =====================================================
    if (opcode == 0xFF) {
        d.type = InstrType::HALT;
        return d;
    }

    // Unknown opcode
    return d;
}
