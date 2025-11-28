#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cstdint>

// Correct tokenizer: DOES NOT break hex values
std::vector<std::string> tokenize(const std::string &line) {
    std::vector<std::string> tokens;
    std::string temp;

    std::istringstream iss(line);

    while (iss >> temp) {
        if (!temp.empty() && temp.back() == ',')
            temp.pop_back();
        tokens.push_back(temp);
    }

    return tokens;
}

uint16_t parse_value(const std::string &s) {
    if (s.rfind("0x", 0) == 0 || s.rfind("0X", 0) == 0)
        return (uint16_t)std::stoi(s, nullptr, 16);
    return (uint16_t)std::stoi(s);
}

int parse_register(const std::string &r) {
    return r[1] - '0';
}

std::unordered_map<std::string, uint8_t> OPCODES = {
    {"MOVI", 0x10},
    {"MOV",  0x11},
    {"ADD",  0x20},
    {"SUB",  0x21},
    {"AND",  0x22},
    {"OR",   0x23},
    {"XOR",  0x24},
    {"CMP",  0x25},
    {"LOAD", 0x30},
    {"STORE",0x31},
    {"JMP",  0x40},
    {"JZ",   0x41},
    {"JNZ",  0x42},
    {"HALT", 0xFF}
};

void pass1(const std::string &file,
           std::vector<std::vector<std::string>>& lines,
           std::unordered_map<std::string, uint16_t>& labels)
{
    std::ifstream f(file);
    if (!f.is_open()) {
        std::cerr << "Cannot open " << file << "\n";
        exit(1);
    }

    uint16_t pc = 0;
    std::string line;

    while (std::getline(f, line)) {
        if (line.empty() || line[0] == ';') continue;

        if (line.back() == ':') {
            std::string label = line.substr(0, line.size() - 1);
            labels[label] = pc;
            continue;
        }

        auto toks = tokenize(line);
        if (!toks.empty()) {
            lines.push_back(toks);
            pc += 5;
        }
    }
}

std::vector<uint8_t> pass2(
    const std::vector<std::vector<std::string>>& lines,
    const std::unordered_map<std::string, uint16_t>& labels)
{
    std::vector<uint8_t> out;

    for (auto &t : lines) {
        std::string op = t[0];
        uint8_t code = OPCODES[op];

        uint16_t op1 = 0;
        uint16_t op2 = 0;

        if (op == "MOVI") {
            op1 = parse_register(t[1]);
            op2 = parse_value(t[2]);
        }
        else if (op == "MOV") {
            op1 = parse_register(t[1]);
            op2 = parse_register(t[2]);
        }
        else if (op == "STORE" || op == "LOAD") {
            op1 = parse_register(t[1]);
            std::string addr = t[2];
            op2 = labels.count(addr) ? labels.at(addr) : parse_value(addr);
        }
        else if (op == "ADD" || op == "SUB" || op=="AND" || op=="OR" || op=="XOR" || op=="CMP") {
            op1 = parse_register(t[1]);
            op2 = parse_register(t[2]);
        }
        else if (op == "JMP" || op == "JZ" || op == "JNZ") {
            std::string tgt = t[1];
            op1 = labels.count(tgt) ? labels.at(tgt) : parse_value(tgt);
        }

        out.push_back(code);
        out.push_back(op1 & 0xFF);
        out.push_back((op1 >> 8) & 0xFF);
        out.push_back(op2 & 0xFF);
        out.push_back((op2 >> 8) & 0xFF);
    }

    return out;
}

int main(int argc, char** argv) {

    if (argc < 3) {
        std::cout << "Usage: ./assembler input.asm output.bin\n";
        return 1;
    }

    std::string in = argv[1];
    std::string outp = argv[2];

    std::vector<std::vector<std::string>> lines;
    std::unordered_map<std::string, uint16_t> labels;

    pass1(in, lines, labels);
    auto data = pass2(lines, labels);

    std::ofstream out(outp, std::ios::binary);
    out.write((char*)data.data(), data.size());
    out.close();

    std::cout << "Assembly successful! Output written to: " << outp << "\n";
    return 0;
}
