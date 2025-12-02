#include "control.h"

// ================================================
// decode()
// Maps opcode + operands into a structured format
// ================================================
DecodedInstr ControlUnit::decode(uint8_t opcode, uint16_t op1, uint16_t op2)
{
    DecodedInstr d{};
    d.type = InstrType::NONE;
    d.alu_op = ALUOp::NONE;

    // ============================
    // MOV Rn, imm
    // ============================
    if (opcode == 0x10) {
        d.type = InstrType::REG_IMM;
        d.rd = op1;
        d.imm = op2;
        d.alu_op = ALUOp::MOV;
        return d;
    }

    // ============================
    // MOV Rn, Rm
    // ============================
    if (opcode == 0x11) {
        d.type = InstrType::REG_REG;
        d.rd = op1;
        d.rs = op2;
        d.alu_op = ALUOp::MOV;
        return d;
    }

    // ============================
    // ALU OPS
    // ============================
    if (opcode == 0x20) { d.type = InstrType::ALU_REG_REG; d.alu_op = ALUOp::ADD; d.rd = op1; d.rs = op2; return d; }
    if (opcode == 0x21) { d.type = InstrType::ALU_REG_REG; d.alu_op = ALUOp::SUB; d.rd = op1; d.rs = op2; return d; }
    if (opcode == 0x22) { d.type = InstrType::ALU_REG_REG; d.alu_op = ALUOp::AND_; d.rd = op1; d.rs = op2; return d; }
    if (opcode == 0x23) { d.type = InstrType::ALU_REG_REG; d.alu_op = ALUOp::OR_; d.rd = op1; d.rs = op2; return d; }
    if (opcode == 0x24) { d.type = InstrType::ALU_REG_REG; d.alu_op = ALUOp::XOR_; d.rd = op1; d.rs = op2; return d; }
    if (opcode == 0x25) { d.type = InstrType::ALU_REG_REG; d.alu_op = ALUOp::CMP; d.rd = op1; d.rs = op2; return d; }

    // ============================
    // LOAD / STORE
    // ============================
    if (opcode == 0x30) {
        d.type = InstrType::LOAD_WORD;
        d.rd = op1;
        d.imm = op2;
        return d;
    }

    if (opcode == 0x31) {
        d.type = InstrType::STORE_WORD;
        d.rs = op1;
        d.imm = op2;
        return d;
    }

    // ============================
    // JMP
    // ============================
    if (opcode == 0x40) {
        d.type = InstrType::JUMP;
        d.imm = op1;
        return d;
    }

    // ============================
    // JZ / JNZ
    // ============================
    if (opcode == 0x41) { d.type = InstrType::JUMP_COND; d.imm = op1; return d; }
    if (opcode == 0x42) { d.type = InstrType::JUMP_COND; d.imm = op1; return d; }

    // ============================
    // PUSH Rn
    // ============================
    if (opcode == 0x50) {
        d.type = InstrType::PUSH_REG;
        d.rs = op1;
        return d;
    }

    // ============================
    // POP Rn
    // ============================
    if (opcode == 0x51) {
        d.type = InstrType::POP_REG;
        d.rd = op1;
        return d;
    }

    // ============================
    // CALL label
    // ============================
    if (opcode == 0x60) {
        d.type = InstrType::CALL;
        d.imm = op1;
        return d;
    }

    // ============================
    // RET
    // ============================
    if (opcode == 0x61) {
        d.type = InstrType::RET;
        return d;
    }

    // ============================
    // HALT
    // ============================
    if (opcode == 0xFF) {
        d.type = InstrType::HALT;
        return d;
    }

    return d;
}
