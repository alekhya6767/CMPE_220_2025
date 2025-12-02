#include "cpu.h"
#include <iostream>
#include <iomanip>

// =======================================
// Constructor
// =======================================

CPU::CPU() {
    regs.PC = 0;
    regs.SP = 0x8000;      // <--- THIS FIXES THE FACTORIAL BUG
    for (int i = 0; i < 8; i++) regs.R[i] = 0;
    regs.flags = {0,0};
}


// =======================================
// Load program into memory and set PC
// =======================================
void CPU::load_program(const std::vector<uint8_t> &program, uint16_t start)
{
    for (size_t i = 0; i < program.size(); i++)
        memory.write8(start + i, program[i]);

    regs.PC = start;
}

// =======================================
// Main execution loop
// =======================================
void CPU::run() {
    while (true) {
        step();
        memory.tick_timer();
    }
}

// =======================================
// Execute a single instruction
// Fetch → Decode → Execute → Update PC
// =======================================
void CPU::step()
{
    // -------- FETCH OPCODE --------
    uint16_t pc = regs.PC;
    uint8_t opcode = memory.read8(pc);
    regs.PC++;

    // -------- FETCH OPERANDS --------
    uint16_t op1 = memory.read16(regs.PC);
    regs.PC += 2;

    uint16_t op2 = memory.read16(regs.PC);
    regs.PC += 2;

    // -------- DECODE --------
    DecodedInstr instr = cu.decode(opcode, op1, op2);

    // -------- EXECUTE --------
    switch (instr.type)
    {
        // =============================
        // MOV Rn, imm
        // =============================
        case InstrType::REG_IMM:
            regs.R[instr.rd] = instr.imm;
            regs.flags.ZF = (instr.imm == 0);
            break;

        // =============================
        // MOV Rn, Rm
        // =============================
        case InstrType::REG_REG:
        {
            uint16_t val = regs.R[instr.rs];
            regs.R[instr.rd] = val;
            regs.flags.ZF = (val == 0);
            break;
        }

        // =============================
        // ALU ops: ADD, SUB, CMP, etc.
        // =============================
        case InstrType::ALU_REG_REG:
        {
            uint16_t a = regs.R[instr.rd];
            uint16_t b = regs.R[instr.rs];
            uint16_t result = 0;

            switch (instr.alu_op)
            {
                case ALUOp::ADD: result = alu.add(a, b, regs.flags); break;
                case ALUOp::SUB: result = alu.sub(a, b, regs.flags); break;
                case ALUOp::AND_: result = alu._and(a, b, regs.flags); break;
                case ALUOp::OR_:  result = alu._or(a, b, regs.flags); break;
                case ALUOp::XOR_: result = alu._xor(a, b, regs.flags); break;
                case ALUOp::CMP:
                    alu.cmp(a, b, regs.flags);
                    break;
                default: break;
            }

            if (instr.alu_op != ALUOp::CMP)
                regs.R[instr.rd] = result;

            break;
        }

        // =============================
        // LOAD / STORE
        // =============================
        case InstrType::LOAD_WORD:
            regs.R[instr.rd] = memory.read16(instr.imm);
            break;

        case InstrType::STORE_WORD:
            memory.write16(instr.imm, regs.R[instr.rs]);
            break;

        // =============================
        // JUMP
        // =============================
        case InstrType::JUMP:
            regs.PC = instr.imm;
            break;

        // =============================
        // JZ / JNZ
        // =============================
        case InstrType::JUMP_COND:
            if (opcode == 0x41 && regs.flags.ZF)  // JZ
                regs.PC = instr.imm;
            else if (opcode == 0x42 && !regs.flags.ZF)  // JNZ
                regs.PC = instr.imm;
            break;

        // =============================
        // PUSH Rn
        // =============================
        case InstrType::PUSH_REG:
        {
            regs.SP -= 2;
            memory.write16(regs.SP, regs.R[instr.rs]);
            break;
        }

        // =============================
        // POP Rn
        // =============================
        case InstrType::POP_REG:
        {
            regs.R[instr.rd] = memory.read16(regs.SP);
            regs.SP += 2;
            break;
        }

        // =============================
        // CALL address
        // =============================
        case InstrType::CALL:
        {
            regs.SP -= 2;
            memory.write16(regs.SP, regs.PC);  // push return PC
            regs.PC = instr.imm;               // jump to function
            break;
        }

        // =============================
        // RET
        // =============================
        case InstrType::RET:
        {
            uint16_t retAddr = memory.read16(regs.SP); // pop PC
            regs.SP += 2;
            regs.PC = retAddr;
            break;
        }


        
        // =============================
        // HALT
        // =============================
        case InstrType::HALT:
            std::cout << "\nCPU HALTED.\n";
            regs.dump();
            memory.dump(0, 0x0060);
            exit(0);

        default:
            break;
    }
}
