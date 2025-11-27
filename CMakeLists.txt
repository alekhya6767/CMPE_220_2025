cmake_minimum_required(VERSION 3.10)
project(SoftwareCPU)

set(CMAKE_CXX_STANDARD 17)

# ========================
# CPU Library
# ========================
add_library(cpu
    cpu/cpu.cpp
    cpu/registers.cpp
    memory/memory.cpp
    control/control.cpp
    alu/alu.cpp
)

target_include_directories(cpu PUBLIC
    cpu
    memory
    control
    alu
)

# ========================
# Emulator Executable
# ========================
add_executable(emulator
    emulator/main.cpp
)

target_link_libraries(emulator cpu)

# ========================
# Assembler Executable
# ========================
add_executable(assembler
    assembler/assembler.cpp
)
