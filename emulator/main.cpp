// ========================================================
// main.cpp – Emulator Entry Point
// This file loads a compiled .bin program, initializes CPU,
// and starts execution until HALT.
// ========================================================

#include <iostream>          // For std::cout, std::cerr
#include <fstream>           // For std::ifstream (loading binary)
#include <vector>            // For std::vector container
#include <string>            // For std::string
#include "../cpu/cpu.h"      // Include CPU class

// ========================================================
// read_binary_file()
// Loads a .bin file into a vector<uint8_t>
// ========================================================
std::vector<uint8_t> read_binary_file(const std::string &filename) {

    // Open file in binary mode, read-only
    std::ifstream file(filename, std::ios::binary);

    // Check if file opened successfully
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open program file: " << filename << "\n";
        exit(1);
    }

    // Seek to end to get file size
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();

    // Resize vector to hold program data
    std::vector<uint8_t> buffer(size);

    // Seek back to start and read bytes
    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), size);

    file.close();
    return buffer;
}

// ========================================================
// main()
// Usage: ./emulator program.bin
// ========================================================
int main(int argc, char** argv) {

    // ----------------------------------------------------
    // Check command-line arguments
    // ----------------------------------------------------
    if (argc < 2) {
        std::cerr << "Usage: ./emulator <program.bin>\n";
        return 1;
    }

    // Store program filename
    std::string program_path = argv[1];

    // ----------------------------------------------------
    // Load program into a byte vector
    // ----------------------------------------------------
    std::vector<uint8_t> program = read_binary_file(program_path);

    // ----------------------------------------------------
    // Create CPU instance
    // ----------------------------------------------------
    CPU cpu;

    // ----------------------------------------------------
    // Load program at address 0x0000
    // ----------------------------------------------------
    cpu.load_program(program, 0x0000);

    std::cout << "Program loaded. Starting CPU...\n\n";

    // ----------------------------------------------------
    // Begin execution loop
    // CPU will run until HALT instruction terminates
    // ----------------------------------------------------
    cpu.run();

    return 0; // Never reached, HALT calls exit()
}
