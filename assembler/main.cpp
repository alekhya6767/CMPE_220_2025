#include "assembler.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: assembler <input.asm> <output.bin>\n";
        return 1;
    }

    std::string inputFile  = argv[1];
    std::string outputFile = argv[2];

    Assembler assembler;
    if (!assembler.assemble(inputFile, outputFile)) {
        std::cerr << "Assembly failed.\n";
        return 1;
    }

    return 0;
}
