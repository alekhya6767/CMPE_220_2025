#include "cpu.h"
#include <iostream>
#include <iomanip>

// ======================================================
// CPU Constructor
// ======================================================
CPU::CPU() {
    // RegisterFile + Memory constructors already initialize everything.
}

// ======================================================
// load_program()
// Load binary program into memory starting at 'start'.
// ======================================================
void CPU::load_program(const std::vector<uint8_t>& program, uint16_t start) {

    for (size_t i = 0; i < program.size(); i++) {
        memory.write8(start + i, program[i]);
    }

    // Execution starts at 'start'
    regs.PC = start;
}

// ======================================================
// run()
// Main loop: step() until HALT instruction triggers exit()
// ======================================================
void CPU::run() {
    while (true) {
        step();
        memory.tick_timer();
    }
}

// ======================================================
// step()
// Fetch → Decode → Execute → Update PC
// ======================================================
void CPU::step() {

    // ----------------------------
    // FETCH OPCODE
    // ----------------------------
    uint16_t pc = regs.PC;
    uint8_t opcode = memory.read8(pc);

    // PC moves to operands
    regs.PC++;

    // ----------------------------
    // FETCH OPERANDS (always 2 words)
    // ----------------------------
    uint16_t op1 = memory.read16(regs.PC);
    regs.PC += 2;

    uint16_t op2 = memory.read16(regs.PC);
    regs.PC += 2;

    // ----------------------------
    // DECODE
    // ----------------------------
    DecodedInstr instr = cu.decode(opcode, op1, op2);

    // ----------------------------
    // EXECUTE
    // ----------------------------
    switch (instr.type) {

        // =====================================================
        // MOVI Rd, #imm
        // =====================================================
        case InstrType::REG_IMM: {
            uint16_t result = alu.mov(instr.imm);
            regs.R[instr.rd] = result;
            regs.flags.ZF = (result == 0);
            break;
        }

        // =====================================================
        // MOV Rd, Rs
        // =====================================================
        case InstrType::REG_REG: {
            uint16_t result = alu.mov(regs.R[instr.rs]);
            regs.R[instr.rd] = result;
            regs.flags.ZF = (result == 0);
            break;
        }

        // =====================================================
        // ALU OPERATIONS: ADD, SUB, AND, OR, XOR, CMP
        // =====================================================
        case InstrType::ALU_REG_REG: {
            uint16_t a = regs.R[instr.rd];
            uint16_t b = regs.R[instr.rs];
            uint16_t result = 0;

            switch (instr.alu_op) {

                case ALUOp::ADD:
                    result = alu.add(a, b, regs.flags);
                    regs.R[instr.rd] = result;
                    break;

                case ALUOp::SUB:
                    result = alu.sub(a, b, regs.flags);
                    regs.R[instr.rd] = result;
                    break;

                case ALUOp::AND_:
                    result = alu._and(a, b, regs.flags);
                    regs.R[instr.rd] = result;
                    break;

                case ALUOp::OR_:
                    result = alu._or(a, b, regs.flags);
                    regs.R[instr.rd] = result;
                    break;

                case ALUOp::XOR_:
                    result = alu._xor(a, b, regs.flags);
                    regs.R[instr.rd] = result;
                    break;

                case ALUOp::CMP:
                    alu.cmp(a, b, regs.flags);
                    break;

                default:
                    break;
            }
            break;
        }

        // =====================================================
        // LOAD Rd, [addr]
        // =====================================================
        case InstrType::LOAD_WORD: {
            uint16_t addr = instr.imm;
            uint16_t val = memory.read16(addr);
            regs.R[instr.rd] = val;
            regs.flags.ZF = (val == 0);
            break;
        }

        // =====================================================
        // STORE Rs, [addr]
        // FIXED: Write only LOW 8 bits, NOT 16-bit
        // =====================================================
        case InstrType::STORE_WORD: {
            uint16_t addr = instr.imm;
            uint16_t val  = regs.R[instr.rs];

            // IMPORTANT FIX:
            // DO NOT WRITE16 — this corrupts the next instruction.
            memory.write8(addr, val & 0xFF);

            break;
        }

        // =====================================================
        // JMP addr
        // =====================================================
        case InstrType::JUMP: {
            regs.PC = instr.imm;
            break;
        }

        // =====================================================
        // JZ / JNZ addr
        // =====================================================
        case InstrType::JUMP_COND: {

            if (opcode == 0x41) {       // JZ
                if (regs.flags.ZF)
                    regs.PC = instr.imm;
            }
            else if (opcode == 0x42) {  // JNZ
                if (!regs.flags.ZF)
                    regs.PC = instr.imm;
            }
            break;
        }

        // =====================================================
        // HALT
        // =====================================================
        case InstrType::HALT: {
            std::cout << "\nCPU HALTED.\n";
            regs.dump();
            memory.dump(0x0000, 0x0030);
            exit(0);
        }

        default:
            break;
    }
}
