// main.cpp
// This is the entry point for the Virtual Machine application.
// It defines and runs various test programs to validate VM functionality.

#include "RohitVM.hpp"     // Includes all VM-related classes (CPU, Memory, Instruction, etc.)
#include <vector>          // Used to create dynamic arrays for instructions
#include <sstream>         // Used to build output as a string
#include <iostream>        // Used for input/output (std::cout)

// =============================================================================
// FUNCTION: runProgram
// Purpose: Loads a given program (a sequence of instructions) into the VM,
//          runs it, and returns the result/output as a string.
// =============================================================================
std::string runProgram(const std::vector<Instruction>& prog, const std::string& title) {
    VM vm;  // Create an instance of the Virtual Machine
    std::ostringstream out; // String stream to collect formatted output

    // Print the program title and formatting
    out << "===============================\n";
    out << "Running Program: " << title << "\n";
    out << "===============================\n";

    vm.loadProgram(prog); // Load the program (set of instructions) into memory
    vm.execute();         // Begin execution of the program (fetch-decode-execute loop)

    return out.str();     // Return the result as a string
}

// =============================================================================
// FUNCTION: main
// Purpose: Entry point of the application. It defines and runs several test programs
//          for the virtual machine and displays their output.
// =============================================================================
int main() {
    // Vector of test programs: each has a title and a sequence of VM instructions
    std::vector<std::pair<std::string, std::vector<Instruction>>> testPrograms = {
        {
            "Basic MOV and HLT",
            {
                // Load value into AX and halt
                {Opcode::MOV, 0x1234},     // MOV AX, 0x1234
                {Opcode::HLT}              // HLT (Stop execution)
            }
        },
        {
            "PUSH & POP",
            {
                // Demonstrate stack operations
                {Opcode::MOV, 0xABCD},     // MOV AX, 0xABCD
                {Opcode::PUSH, 0x00},      // PUSH AX
                {Opcode::POP,  0x01},      // POP BX
                {Opcode::HLT}
            }
        },
        {
            "Flag Set/Clear",
            {
                // Demonstrate flag manipulation
                {Opcode::STE},             // Set Equal flag
                {Opcode::STG},             // Set Greater flag
                {Opcode::STH},             // Set High-bit flag
                {Opcode::STL},             // Set Low-bit flag
                {Opcode::CLG},             // Clear Greater flag
                {Opcode::CLL},             // Clear Low-bit flag
                {Opcode::HLT}
            }
        },
        {
            "Multiple MOVs",
            {
                // Load different registers with values
                {Opcode::MOV, 0xAAAA},     // MOV AX, 0xAAAA
                {Opcode::MOV_BX, 0x5005},  // MOV BX, 0x5005
                {Opcode::MOV_CX, 0xF00D},  // MOV CX, 0xF00D
                {Opcode::MOV_DX, 0xDEAD},  // MOV DX, 0xDEAD
                {Opcode::HLT}
            }
        },
        {
            "Arithmetic: ADD AX + BX",
            {
                {Opcode::MOV, 0x0011},     // MOV AX, 0x0011
                {Opcode::MOV_BX, 0x0009},  // MOV BX, 0x0009
                {Opcode::ADD},             // AX = AX + BX (0x0011 + 0x0009 = 0x001A)
                {Opcode::HLT}
            }
        },
        {
            "Arithmetic: SUB AX - BX",
            {
                {Opcode::MOV, 0x0015},     // MOV AX, 0x0015
                {Opcode::MOV_BX, 0x0005},  // MOV BX, 0x0005
                {Opcode::SUB},             // AX = AX - BX (0x0015 - 0x0005 = 0x0010)
                {Opcode::HLT}
            }
        },
        {
            "Arithmetic: MUL AX * BX",
            {
                {Opcode::MOV, 0x0003},     // MOV AX, 0x0003
                {Opcode::MOV_BX, 0x0004},  // MOV BX, 0x0004
                {Opcode::MUL},             // AX = AX * BX (0x0003 * 0x0004 = 0x000C)
                {Opcode::HLT}
            }
        },
        {
            "Arithmetic: DIV AX / BX",
            {
                {Opcode::MOV, 0x0020},     // MOV AX, 0x0020
                {Opcode::MOV_BX, 0x0004},  // MOV BX, 0x0004
                {Opcode::DIV},             // AX = AX / BX (0x0020 / 0x0004 = 0x0008)
                {Opcode::HLT}
            }
        },
        {
            "Arithmetic: DIV by zero (error)",
            {
                {Opcode::MOV, 0x0020},     // MOV AX, 0x0020
                {Opcode::MOV_BX, 0x0000},  // MOV BX, 0x0000
                {Opcode::DIV},             // AX = AX / BX (should trigger error)
                {Opcode::HLT}
            }
        }
    };

    // Loop through each program, run it, and display output
    for (const auto& [title, prog] : testPrograms) {
        std::cout << runProgram(prog, title) << "\n";  // Execute and print each test program
    }

    return 0; // End of main program
}
