#pragma once

#include <vector>
#include <cstdint>
#include "registers.h"
#include "memory.h"
#include "alu.h"
#include "control.h"

class CPU {
public:
    RegisterFile regs;
    Memory memory;
    ALU alu;
    ControlUnit cu;

    CPU();

    void load_program(const std::vector<uint8_t> &program,
                      uint16_t start);

    void run();
    void step();
};
