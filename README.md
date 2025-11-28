# CMPE 220 – Software CPU Project (2025)

This project implements a fully functional **16-bit Software CPU** in C++, capable of running custom assembly programs through an **assembler** and **emulator**. The system includes a complete ISA, CPU architecture, ALU, memory subsystem, control unit, and memory-mapped I/O for console output.

This work was created by **Group 6** as part of the **CMPE 220 – System Software** course at San José State University.

---

## 🚀 Features

### ✔ 16-bit CPU Architecture
- 4 General-purpose registers (`R0`, `R1`, `R2`, `R3`)
- 16-bit Program Counter (`PC`)
- Status Flags (`ZF`, `CF`)
- Memory size: **64 KB**
- Memory-mapped I/O:
  - `0xFF00` → ASCII output port
  - `0xFF01` → Timer/clock

### ✔ ALU (Arithmetic Logic Unit)
Supports:
- `ADD`
- `SUB`
- `AND`
- `OR`
- `XOR`
- `CMP`
- `MOV`

All ALU operations update flags accordingly.

### ✔ Control Unit
Decodes 1-byte opcodes into executable micro-operations.

### ✔ Assembler
Converts `.asm` files into binary (`.bin`)
- Supports labels
- 16-bit immediates
- Register addressing
- Memory addressing

### ✔ Emulator
Executes assembled programs using:
- Fetch → Decode → Execute cycle
- Memory-mapped I/O for printing output
- Timer increment on each instruction


