// RohitVM.hpp
#pragma once  // Ensures this header is only included once during compilation

// Include standard C++ libraries needed for memory, vector storage, exceptions, etc.
#include <cstdint>      // For fixed-width integer types like uint16_t
#include <vector>       // For using std::vector to model memory
#include <memory>       // (Not used here directly, but may be useful for future expansion)
#include <cassert>      // For assertions during development
#include <cstdlib>      // For exit()
#include <cstdarg>      // For variadic functions (not used in this file)
#include <cstdio>       // For printf()
#include <stdexcept>    // For throwing runtime errors

#include "RohitUtils.hpp" // Include custom utility functions (like printhex, copy, etc.)

// ===========================================================================
// Author: Rohit Yadav
// Description: This header file defines the main building blocks of a virtual CPU.
//              It simulates registers, memory, instruction handling, and the VM class.
//              This is the core architectural definition of the 16-bit virtual machine.

// ===========================================================================
// CLASS: Registers
// Simulates the actual CPU registers used for computation and memory addressing.
// ===========================================================================

class Registers {
public:
    // General purpose 16-bit registers commonly found in x86-like architectures.
    uint16_t ax = 0;   // Accumulator Register: Used for arithmetic operations.
    uint16_t bx = 0;   // Base Register: General-purpose, often used in memory access.
    uint16_t cx = 0;   // Count Register: Typically used in loops or shifts.
    uint16_t dx = 0;   // Data Register: Also general-purpose, often paired with AX.

    // Special-purpose registers
    uint16_t sp = 0xFFFF; // Stack Pointer: Points to top of the stack in memory (starts at top).
    uint16_t ip = 0x0000; // Instruction Pointer: Holds the address of the next instruction.
    uint16_t flags = 0x0000; // Flags Register: Stores results of comparisons/conditions (4 bits used here).

    // Enum to represent individual flag bits (used in the FLAGS register)
    enum Flag {
        Equal   = 0x08, // Equal flag (bit 3)
        Greater = 0x04, // Greater flag (bit 2)
        Higher  = 0x02, // High-bit flag (bit 1)
        Lower   = 0x01  // Low-bit flag (bit 0)
    };
};

// ===========================================================================
// CLASS: CPU
// A wrapper for Registers, providing utility functions to read/write FLAGS.
// ===========================================================================

class CPU {
public:
    Registers r;  // 'r' holds all the register values

    // ------------------------
    // Flag Getters (Check if a specific flag is set)
    // ------------------------
    bool isEqual() const   { return r.flags & Registers::Equal; }
    bool isGreater() const { return r.flags & Registers::Greater; }
    bool isHigher() const  { return r.flags & Registers::Higher; }
    bool isLower() const   { return r.flags & Registers::Lower; }

    // ------------------------
    // Flag Setters (Set or clear a specific flag)
    // ------------------------
    void setEqual(bool val)   { setFlag(Registers::Equal, val); }
    void setGreater(bool val) { setFlag(Registers::Greater, val); }
    void setHigher(bool val)  { setFlag(Registers::Higher, val); }
    void setLower(bool val)   { setFlag(Registers::Lower, val); }

private:
    // Internal utility to set/clear any flag bit using bit masking
    void setFlag(uint16_t mask, bool val) {
        if (val)
            r.flags |= mask;    // Set bit
        else
            r.flags &= ~mask;   // Clear bit
    }
};

// ===========================================================================
// CLASS: Memory
// A class that simulates 64KB (65,536 bytes) of memory as a vector of bytes.
// ===========================================================================

class Memory {
public:
    static constexpr size_t SIZE = 65536; // Total memory size (16-bit addressable space)
    std::vector<uint8_t> data;            // Vector used to store memory bytes

    // Constructor: Initialize memory with 0s
    Memory() : data(SIZE, 0) {}

    // Operator Overloading to allow memory[address] access (read/write)
    uint8_t& operator[](uint16_t addr) {
        return data.at(addr); // 'at()' does bounds checking
    }

    const uint8_t& operator[](uint16_t addr) const {
        return data.at(addr); // const version (read-only)
    }

    // Returns raw pointer to beginning of memory array (useful for printing, copying, etc.)
    uint8_t* raw() { return data.data(); }
};

// ===========================================================================
// ENUM: Opcode
// Defines all supported operations (instructions) for the VM to understand.
// ===========================================================================

enum class Opcode : uint8_t {
    // Basic Instructions
    NOP = 0x01,      // No Operation
    HLT = 0x02,      // Halt the program

    // Register Move Instructions
    MOV = 0x08,      // MOV AX, value
    MOV_BX = 0x09,   // MOV BX, value
    MOV_CX = 0x0A,   // MOV CX, value
    MOV_DX = 0x0B,   // MOV DX, value
    MOV_SP = 0x0C,   // MOV SP, value

    // Flag Set/Clear Instructions
    STE = 0x10, CLE = 0x11,   // Set/Clear Equal flag
    STG = 0x12, CLG = 0x13,   // Set/Clear Greater flag
    STH = 0x14, CLH = 0x15,   // Set/Clear Higher flag
    STL = 0x16, CLL = 0x17,   // Set/Clear Lower flag

    // Stack Instructions
    PUSH = 0x1A,      // PUSH register
    POP = 0x1B,       // POP to register

    // Arithmetic Instructions
    ADD = 0x20,       // ADD AX, BX => AX = AX + BX
    SUB = 0x21,       // SUB AX, BX => AX = AX - BX
    MUL = 0x22,       // MUL AX, BX => AX = AX * BX
    DIV = 0x23        // DIV AX, BX => AX = AX / BX (if BX != 0)
};

// ===========================================================================
// STRUCT: Instruction
// A single instruction object with its opcode and operands.
// a1 and a2 are optional 16-bit operands depending on instruction.
// ===========================================================================

struct Instruction {
    Opcode op;       // The operation code (what kind of instruction)
    uint16_t a1 = 0; // First operand (e.g., a register value)
    uint16_t a2 = 0; // Second operand (if applicable)
};

// ===========================================================================
// CLASS: VM (Virtual Machine)
// The main class that brings together CPU, Memory, and Instruction Execution
// ===========================================================================

class VM {
public:
    CPU cpu;                // Holds registers and flag logic
    Memory memory;          // Holds 64KB of program memory
    uint16_t breakLine = 0; // Used to track where the next instruction should be placed

    // Constructor
    VM() = default;

    // Public functions to load and run a program
    void execute();  // Main function to start execution (fetch-decode-execute loop)
    void loadProgram(const std::vector<Instruction>& program); // Load a program into memory

private:
    // Internal helper functions used by the VM
    void executeInstruction(const Instruction& instr); // Executes one instruction
    void handleError(const std::string& msg, bool fatal = true); // Reports errors
    uint8_t getInstructionSize(Opcode op); // Returns size of instruction in bytes
    void push(uint16_t val); // Push value onto the stack
    uint16_t pop();          // Pop value from the stack
    Instruction fetchNextInstruction(); // Read next instruction from memory
};
