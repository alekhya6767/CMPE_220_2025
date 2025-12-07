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

### ✔ Repository Structure

alu/ – Arithmetic Logic Unit operations (ADD, SUB, AND, OR, XOR, CMP, MOV)

control/ – Control Unit: decodes opcodes into micro-operations

cpu/ – CPU core: registers, flags, PC, SP, and fetch/decode/execute loop

memory/ – 64 KB memory model and memory-mapped I/O (0xFF00 output, 0xFF01 timer)

assembler/ – Assembler that converts .asm source into .bin machine code

emulator/ – Emulator entry point; loads .bin files and runs the CPU

programs/ – Sample assembly and C programs (e.g., factorial.asm, factorial.c)

docs/ – Project documentation (reports, ISA/design documents)

CMakeLists.txt – CMake build configuration for the whole project


### ✔ How to Download, Compile, and Run 

1. Clone the Repository
git clone https://github.com/alekhya6767/CMPE_220_2025.git 
cd CMPE_220_2025

2. Build the Project
mkdir build 
cd build 
cmake .. 
make

3. Assemble a Program and Run the Program in the Emulator

Fibonacci program: 
./assembler ../programs/fib.asm fib.bin

./emulator fib.bin

Hello World program: 
./assembler ../programs hello.asm hello.bin

./emulator hello.bin

Factorial Program:
./assembler ../programs/factorial.asm factorial.bin

./emulator factorial.bin

4. Run the C Program
cd programs
gcc factorial.c -o factorial_c
./factorial_c


### ✔ Team Member Contributions:

### Software and CPU Design:

Divyasri Lakshmi Alekhya Nakka

• Designed CPU architecture (Registers, ALU, Control Unit, Memory Map)
• Implemented CPU instruction execution logic (cpu.cpp)
• Implemented Memory module with memory-mapped I/O
• Wrote example programs (Fibonacci, Hello World)
• Created diagrams and documentation
• Recorded demo video

Pooja Ramesh Sindham

• Implemented ALU operations (ADD, SUB, CMP, AND, OR, XOR, MOV)
• Wrote flag handling logic (ZF, CF)
• Helped design instruction formats and ISA spec
• Performed debugging and test execution

Chanukya Vejandla

• Implemented Control Unit (control.cpp) with opcode decoding
• Added JUMP, JZ, JNZ logic
• Integrated assembler with instruction encoding
• Wrote memory dump and register dump features

Sreya Atluri

• Developed assembler (assembler.cpp)
• Added support for labels, immediates, numeric literals
• Ensured correct binary encoding
• Helped create README and documentation
• Helped test Fibonacci and Hello World programs


### ✔ Team Member Contributions:

### Program Layout & Execution:

Divyasri Lakshmi Alekhya Nakka

• Wrote the recursive factorial.asm program using CALL and RET.
• Implemented base case (n == 0 → 1) and recursive case (n * fact(n-1)).
• Debugged the program using emulator output and register/memory dumps.
• Verified that the correct result is printed through memory-mapped I/O.
• Recorded demo video

Pooja Ramesh Sindham

• Helped define how function arguments and return values are passed in registers.
• Checked stack behavior during recursive calls (push/pop and return addresses).
• Tested different input values to confirm correct recursion and results.

Chanukya Vejandla

• Ensured the assembler correctly encodes CALL, RET, PUSH, and POP.
• Fixed issues with labels and instruction operands in the factorial binary.
• Rebuilt and ran factorial.bin on the emulator to confirm correct control flow.
 
Sreya Atluri

• Described how the factorial program is laid out in memory (code + stack + I/O).
• Drew or explained the recursion steps for an example (e.g., fact(5)).
• Helped write the explanation of how the stack grows and shrinks during recursion.


