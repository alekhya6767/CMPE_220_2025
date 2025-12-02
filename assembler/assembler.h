#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

class Assembler {
public:
    bool assemble(const std::string &inputFile, const std::string &outputFile);

private:
    std::unordered_map<std::string, uint16_t> labels;

    bool first_pass(const std::vector<std::string> &lines);
    bool second_pass(const std::vector<std::string> &lines, std::vector<uint8_t> &output);

    void encode_instruction(uint8_t opcode, uint16_t op1, uint16_t op2,
                            std::vector<uint8_t> &out);

    // Helpers
    uint16_t parse_number(const std::string &s);
    int get_register_index(const std::string &reg);
};
