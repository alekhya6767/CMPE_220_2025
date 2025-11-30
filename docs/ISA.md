# 16-bit CPU Instruction Set Architecture (ISA)

## 1. CPU Overview

-   **Architecture:** 16-bit
-   **Registers:**
    -   **R0 -- R3**: 4 general-purpose 16-bit registers
    -   **PC**: 16-bit Program Counter (byte address)
    -   **Flags**: Status register containing:
        -   **ZF** -- Zero Flag
        -   **CF** -- Carry Flag
-   **Memory:** 64 KB (addresses `0x0000` -- `0xFFFF`)

### Memory-mapped I/O

-   `0xFF00` -- ASCII output port
    -   Writing a byte here prints the corresponding character.
-   `0xFF01` -- Timer/clock
    -   Incremented once per executed instruction.\
    -   Reading from this address returns the current tick count.

The emulator must treat reads/writes to these two addresses specially.

------------------------------------------------------------------------

## 2. Instruction Format

Instructions are encoded as **4 bytes (32 bits)** in memory, even though
the CPU data path is 16-bit.

``` text
[ Byte 0 | Byte 1 | Byte 2 | Byte 3 ]
[ Opcode |   Op1  |   Op2  |   Op3  ]
```

-   **Byte 0 -- Opcode (8 bits)**
-   **Bytes 1--3 -- Operands (8 bits each)**

### 2.1 Register--Register format

    Byte 0: Opcode
    Byte 1: Dest register (0–3)
    Byte 2: Source register (0–3)
    Byte 3: 0

### 2.2 Register + 16-bit Immediate format

    Byte 0: Opcode
    Byte 1: Dest register
    Byte 2: Imm[15:8]
    Byte 3: Imm[7:0]

### 2.3 Register + 16-bit Address format

    Byte 0: Opcode
    Byte 1: Register
    Byte 2: Addr[15:8]
    Byte 3: Addr[7:0]

------------------------------------------------------------------------

## 3. Registers

-   **R0--R3**: 4 general-purpose 16-bit registers\
-   **PC**: increments by 4\
-   **Flags**: ZF, CF

------------------------------------------------------------------------

## 4. Addressing Modes

-   Register\
-   Immediate 16-bit\
-   Memory direct `[0x8000]`\
-   Memory-mapped I/O

------------------------------------------------------------------------

## 5. Instruction Set

### 5.1 ALU

  Opcode   Mnemonic   Description
  -------- ---------- ----------------
  0x00     NOP        Do nothing
  0x01     ADD        Rd = Rd + Rs
  0x02     SUB        Rd = Rd - Rs
  0x03     AND        Rd = Rd & Rs
  0x04     OR         Rd = Rd \| Rs
  0x05     XOR        Rd = Rd \^ Rs
  0x06     CMP        Compare Rd, Rs
  0x07     MOV        Rd = Rs
  0x08     MOVI       Rd = Imm16

### 5.2 Memory

  Opcode   Mnemonic
  -------- ----------
  0x09     LOAD
  0x0A     STORE

### 5.3 Control Flow

  Opcode   Mnemonic   Description
  -------- ---------- ----------------
  0x0B     JMP        Jump
  0x0C     JZ         If ZF=1 jump
  0x0D     JNZ        If ZF=0 jump
  0x0E     JC         If CF=1 jump
  0x0F     JNC        If CF=0 jump
  0x10     HALT       Stop execution

------------------------------------------------------------------------

## 6. Flag Semantics

ADD updates ZF, CF.\
SUB updates ZF, CF.\
Logic clears CF.

------------------------------------------------------------------------

## 7. Assembler

-   Supports labels\
-   16-bit immediates\
-   Register names R0--R3\
-   Creates .bin output

------------------------------------------------------------------------

## 8. Emulator Execution

-   Fetch 4 bytes\
-   Decode opcode + operands\
-   Execute ALU / MEM / CTRL\
-   Increment PC += 4 unless jumped\
-   Timer at 0xFF01 increments each cycle\
-   HALT stops execution
