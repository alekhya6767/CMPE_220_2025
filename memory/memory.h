#pragma once

#include <cstdint>
#include <vector>
#include <iostream>
#include "common.h"

class Memory {
private:
    std::vector<uint8_t> mem;

public:
    Memory();

    uint8_t read8(uint16_t addr) const;
    uint16_t read16(uint16_t addr) const;

    void write8(uint16_t addr, uint8_t value);
    void write16(uint16_t addr, uint16_t value);

    void tick_timer();    // <-- MUST MATCH EXACTLY
    void dump(uint16_t start, uint16_t end) const;  // <-- MUST MATCH EXACTLY
};
