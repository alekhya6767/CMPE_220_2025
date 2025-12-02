#include "assembler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

// ------------------------------------------------------------
// Trim helper
// ------------------------------------------------------------
static std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// ------------------------------------------------------------
// Remove comments
// ------------------------------------------------------------
static std::string remove_comment(const std::string &line) {
    size_t p = line.find(';');
    if (p == std::string::npos) return line;
    return line.substr(0, p);
}

// ------------------------------------------------------------
// SPLIT instruction safely ("MOV R1, R2")
// ------------------------------------------------------------
static std::vector<std::string> tokenize(const std::string &line) {
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word) {
        if (!word.empty() && word.back() == ',')
            word.pop_back();
        out.push_back(word);
    }
    return out;
}

// ============================================================
// PASS 1: Collect label addresses
// ============================================================
bool Assembler::assemble(const std::string &inputFile, const std::string &outputFile)
{
    std::ifstream fin(inputFile);
    if (!fin.is_open()) {
        std::cerr << "Error: Cannot open ASM file.\n";
        return false;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(fin, line))
        lines.push_back(line);
    fin.close();

    if (!first_pass(lines)) return false;

    std::vector<uint8_t> output;
    if (!second_pass(lines, output)) return false;

    std::ofstream fout(outputFile, std::ios::binary);
    fout.write((char*)output.data(), output.size());
    fout.close();

    std::cout << "Assembly successful! Output written to: " << outputFile << "\n";
    return true;
}

bool Assembler::first_pass(const std::vector<std::string> &lines)
{
    labels.clear();
    uint16_t pc = 0;

    for (auto &raw : lines) {
        std::string line = trim(remove_comment(raw));
        if (line.empty()) continue;

        // Label
        if (line.back() == ':') {
            std::string label = trim(line.substr(0, line.size() - 1));
            labels[label] = pc;
            continue;
        }

        // Real instruction → always 5 bytes
        pc += 5;
    }
    return true;
}

// ------------------------------------------------------------
// Convert tokens to opcode
// ------------------------------------------------------------
static uint8_t get_opcode(const std::string &m) {
    if (m == "MOVI") return 0x10;
    if (m == "MOV")  return 0x11;

    if (m == "ADD")  return 0x20;
    if (m == "SUB")  return 0x21;
    if (m == "AND")  return 0x22;
    if (m == "OR")   return 0x23;
    if (m == "XOR")  return 0x24;
    if (m == "CMP")  return 0x25;

    if (m == "LOAD")  return 0x30;
    if (m == "STORE") return 0x31;

    if (m == "JMP") return 0x40;
    if (m == "JZ")  return 0x41;
    if (m == "JNZ") return 0x42;

    if (m == "PUSH") return 0x50;
    if (m == "POP")  return 0x51;

    if (m == "CALL") return 0x60;
    if (m == "RET")  return 0x61;

    if (m == "HALT") return 0xFF;

    return 0; // error handled later
}

// ------------------------------------------------------------
// Convert register token "R4"
// ------------------------------------------------------------
int Assembler::get_register_index(const std::string &reg) {
    if (reg.size() < 2 || reg[0] != 'R')
        throw std::runtime_error("Invalid register: " + reg);
    return std::stoi(reg.substr(1));
}

// ------------------------------------------------------------
// Parse immediate or label
// ------------------------------------------------------------
uint16_t Assembler::parse_number(const std::string &s)
{
    if (s.empty()) return 0;

    if (labels.count(s))
        return labels[s];

    if (s.rfind("0x", 0) == 0)
        return std::stoi(s, nullptr, 16);

    return std::stoi(s);
}

// ============================================================
// PASS 2: Encode instructions
// ============================================================
bool Assembler::second_pass(const std::vector<std::string> &lines,
                            std::vector<uint8_t> &out)
{
    out.clear();

    for (auto &raw : lines) {

        std::string clean = trim(remove_comment(raw));
        if (clean.empty()) continue;

        if (clean.back() == ':') continue;  // skip label

        auto tokens = tokenize(clean);
        if (tokens.empty()) continue;

        std::string mnemonic = tokens[0];
        uint8_t opcode = get_opcode(mnemonic);

        if (opcode == 0) {
            std::cerr << "Unknown instruction: " << mnemonic << "\n";
            return false;
        }

        uint16_t op1 = 0, op2 = 0;

        // ---------------------------- SPECIAL CASES ----------------------------

        if (opcode == 0x61) { // RET
            encode_instruction(opcode, 0, 0, out);
            continue;
        }

        if (opcode == 0x50) { // PUSH Rn
            if (tokens.size() != 2) throw std::runtime_error("PUSH requires 1 operand");
            op1 = get_register_index(tokens[1]);
            op2 = 0;
            encode_instruction(opcode, op1, op2, out);
            continue;
        }

        if (opcode == 0x51) { // POP Rn
            if (tokens.size() != 2) throw std::runtime_error("POP requires 1 operand");
            op1 = get_register_index(tokens[1]);
            op2 = 0;
            encode_instruction(opcode, op1, op2, out);
            continue;
        }

        if (opcode == 0x60) { // CALL label
            if (tokens.size() != 2) throw std::runtime_error("CALL requires 1 operand");
            op1 = parse_number(tokens[1]);
            op2 = 0;
            encode_instruction(opcode, op1, op2, out);
            continue;
        }

        // ---------------------- GENERAL 2-OPERAND INSTRUCTIONS ----------------------

        if (tokens.size() == 2) {
            // Rn, imm OR Rn, Rm
            op1 = (tokens[1][0] == 'R') ?
                get_register_index(tokens[1]) :
                parse_number(tokens[1]);
        }
        else if (tokens.size() == 3) {
            op1 = (tokens[1][0] == 'R') ?
                get_register_index(tokens[1]) :
                parse_number(tokens[1]);

            op2 = (tokens[2][0] == 'R') ?
                get_register_index(tokens[2]) :
                parse_number(tokens[2]);
        }

        encode_instruction(opcode, op1, op2, out);
    }

    return true;
}

// ============================================================
// Encode instruction (5 bytes)
// ============================================================
void Assembler::encode_instruction(uint8_t opcode,
                                   uint16_t op1,
                                   uint16_t op2,
                                   std::vector<uint8_t> &out)
{
    out.push_back(opcode);
    out.push_back(op1 & 0xFF);
    out.push_back((op1 >> 8) & 0xFF);
    out.push_back(op2 & 0xFF);
    out.push_back((op2 >> 8) & 0xFF);
}
