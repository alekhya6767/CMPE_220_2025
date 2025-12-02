# Recursion, Stack, and Memory Layout in Our Software CPU

This document explains how our **Software CPU** executes recursive functions using a **call stack**, how the **executable is laid out in memory**, and how recursion unfolds for our `factorial` program.

We connect three pieces:

- A simple **C reference implementation** (`factorial.c`)
- The equivalent **assembly program** (`programs/factorial.asm`)
- The **CALL/RET + PUSH/POP** implementation in our CPU and memory model

---

## 1. Software CPU Overview

Our Software CPU is a **16-bit, single-address-space machine** implemented in C++.

Key features:

- **Registers**
  - `R0`–`R5` : general-purpose 16-bit registers
  - `PC` : Program Counter (16-bit)
  - `SP` : Stack Pointer (16-bit, grows downward)
  - `ZF` : Zero Flag
  - `CF` : Carry/Borrow Flag
- **Memory**
  - 64 KB (`0x0000` – `0xFFFF`)
  - Unified code + data + stack + memory-mapped I/O
- **Instruction Set (subset used here)**
  - `MOVI Rn, imm`   – move immediate
  - `MOV Rn, Rm`     – register-to-register move
  - `ADD/SUB/CMP`    – arithmetic / compare
  - `STORE Rn, [addr]` – write to memory (including I/O)
  - `JZ`, `JNZ`, `JMP` – control flow
  - `PUSH Rn`, `POP Rn` – stack operations
  - `CALL label`, `RET` – function call/return
  - `HALT` – stop execution

Programs are assembled with `assembler` into `.bin` files and then executed by `emulator`.

---

## 2. Memory Layout of the Executable

We use a very simple, fixed memory layout for our programs:

```text
+------------------------+  0xFFFF
|        (unused)        |
|        ...             |
|   Stack (grows down)   |  <-- SP starts near here (e.g., 0xFFFE)
|        ...             |
+------------------------+
|   Memory-Mapped I/O    |
|   0xFF00: OUTPUT port  |
|   0xFF01: TIMER        |
+------------------------+
|        ...             |
|   Data / Heap (if any) |
|        ...             |
+------------------------+
|   Program Code (.bin)  |  <-- factorial.bin, fib.bin, hello.bin
|   loaded at 0x0000     |
+------------------------+  0x0000
````

### 2.1 Code Segment

* When we run:

  ```bash
  ./emulator factorial.bin
  ```

  the emulator loads `factorial.bin` into **memory starting at address `0x0000`**.
* The CPU sets `PC = 0x0000` before starting execution.

### 2.2 Stack

* `SP` is initialized near the **top of memory** (e.g., `0xFFFE`).
* The stack **grows downward**:

  * `PUSH` and `CALL` **decrement** `SP` and then store data at `[SP]`.
  * `POP` and `RET` **read** from `[SP]` and then **increment** `SP`.

### 2.3 Memory-Mapped I/O

* We reserve `0xFF00` as a simple **output port**:

  ```cpp
  // In memory.cpp
  if (addr == IO_OUTPUT) {
      std::cout << value << " " << std::flush;
      mem[addr] = value;
      return;
  }
  ```

* Any `STORE Rx, 0xFF00` writes the value of `Rx` to the console.

  * For `fib.asm`, this prints Fibonacci numbers.
  * For `hello.asm`, this prints ASCII codes (or characters if we choose to cast to `char`).

---

## 3. CALL/RET and Stack Behavior

We implement **function calls** using `CALL` and `RET`, with support registers stored on the stack by `PUSH`/`POP`.

### 3.1 PUSH and POP

**PUSH Rn**:

```cpp
// InstrType::PUSH_REG
regs.SP -= 2;                          // make room on the stack
memory.write16(regs.SP, regs.R[instr.rs]);  // store Rn at [SP]
```

* Stack grows down.
* The value in `Rn` is saved at the new top of the stack.

**POP Rn**:

```cpp
// InstrType::POP_REG
regs.R[instr.rd] = memory.read16(regs.SP);  // load top of stack into Rn
regs.SP += 2;                               // remove 2 bytes from the stack
```

* Read the last pushed value into `Rn`.
* Move `SP` back up (stack shrinks).

### 3.2 CALL

**CALL label** must:

1. Save the **return address** (the `PC` after the CALL instruction).
2. Jump to the function’s address.

Implementation:

```cpp
// InstrType::CALL
regs.SP -= 2;
memory.write16(regs.SP, regs.PC);  // push return address
regs.PC = instr.imm;               // jump to function
```

* `instr.imm` contains the **absolute address** of the function label (resolved by the assembler in the first pass).
* On `CALL fact`, the CPU:

  * Pushes the return `PC` on the stack.
  * Sets `PC` to the `fact` label address.

### 3.3 RET

**RET** must:

1. Restore the **return address** from the stack.
2. Jump back to the caller.

Implementation:

```cpp
// InstrType::RET
uint16_t retAddr = memory.read16(regs.SP);  // read saved PC
regs.SP += 2;                               // pop
regs.PC = retAddr;                          // return to caller
```

* After `RET`, control flows back to the instruction after the original `CALL`.

---

## 4. Recursive Factorial Example

To illustrate recursion, we use a simple factorial function.

### 4.1 C Reference Implementation

This is the conceptual model:

```c
int fact(int n) {
    if (n == 0) return 1;
    return n * fact(n - 1);
}

int main(void) {
    int n = 5;
    int result = fact(n);
    // result = 120
    return 0;
}
```

Our assembly (`programs/factorial.asm`) implements the same logic using the Software CPU’s instructions.

### 4.2 Calling Convention for fact(n)

We use the following register convention:

* **Input / Output**

  * `R0` – holds `n` at function entry and the **return value** at function exit.
* **Temporaries / Locals**

  * `R1` – copy of `n` used for comparisons or multiplication.
  * `R2`, `R3`, `R4`, `R5`, `R6` – temporaries for arithmetic and loops.
* **Stack**

  * Used to **save `n`** and the **return address** at each recursive call.

High-level algorithm in assembly:

1. Base case: if `n == 0`, return `1` in `R0`.
2. Recursive case:

   * Save `n` on stack with `PUSH R0`.
   * Decrement `n` and `CALL fact`.
   * Restore original `n` from stack with `POP R4`.
   * Multiply `fact(n-1)` by `n` using an addition loop.
   * Return.

### 4.3 Stack Frames During Recursion

Consider `n = 3` to see the call stack clearly:

1. **Initial call**

   * `R0 = 3`
   * `CALL fact`

2. **fact(3)**:

   * `n = 3` is not zero → recursive case.
   * `PUSH R0` → stack now holds `3`.
   * `R0 = 2`; `CALL fact`

3. **fact(2)**:

   * `n = 2` is not zero → recursive case.
   * `PUSH R0` → stack now holds `2`, `3`.
   * `R0 = 1`; `CALL fact`

4. **fact(1)**:

   * `n = 1` is not zero → recursive case.
   * `PUSH R0` → stack now holds `1`, `2`, `3`.
   * `R0 = 0`; `CALL fact`

5. **fact(0)** (base case):

   * `n == 0` → `R0 = 1`.
   * `RET` → returns to `fact(1)` with `R0 = 1`.

6. **Unwinding: fact(1)**:

   * `POP R4` restores saved `n = 1`.
   * Multiply result: `R0 = 1 * 1 = 1`.
   * `RET` → returns to `fact(2)` with `R0 = 1`.

7. **Unwinding: fact(2)**:

   * `POP R4` restores `n = 2`.
   * Multiply result: `R0 = 2 * 1 = 2`.
   * `RET` → returns to `fact(3)` with `R0 = 2`.

8. **Unwinding: fact(3)**:

   * `POP R4` restores `n = 3`.
   * Multiply result: `R0 = 3 * 2 = 6`.
   * `RET` → returns to `main` with `R0 = 6`.

For `n = 5`, the same pattern continues:

* `fact(0)` returns `1`
* `fact(1)` returns `1`
* `fact(2)` returns `2`
* `fact(3)` returns `6`
* `fact(4)` returns `24`
* `fact(5)` returns `120`

In our emulator, after running:

```bash
./assembler ../programs/factorial.asm factorial.bin
./emulator factorial.bin
```

we see the final value **`120`** printed through the output port at `0xFF00`.

---

## 5. Summary

* The executable is loaded at **address `0x0000`**, and `PC` starts there.
* The **stack grows downward** from high memory (near `0xFFFE`), managed by `SP`, `PUSH`, `POP`, `CALL`, and `RET`.
* **Function calls**:

  * `CALL` pushes the return address and jumps to the function.
  * `RET` pops the return address and resumes the caller.
* **Recursion**:

  * Each recursive call to `fact(n)` creates a new implicit “frame” on the stack:

    * Saved argument `n`
    * Saved return address
  * The base case returns `1`, and each stacked call multiplies by its `n` while unwinding.
* This demonstrates how **high-level recursive C code** (`factorial.c`) maps down to our **custom ISA**, **stack layout**, and **control flow** in the Software CPU.


