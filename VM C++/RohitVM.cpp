// RohitVM.cpp
// This file contains the main implementation of the Virtual Machine (VM).
// It defines how instructions are executed, how the program is loaded,
// and how memory/registers/stack are handled during runtime.

#include "RohitVM.hpp"   // Include the corresponding header file with class definitions
#include <iostream>      // For input/output (e.g., printing to console)
#include <map>           // Used to map Opcodes to their instruction sizes

// ---------------------------------------------------------------------------
// Function: VM::execute
// Purpose: This is the main function that runs the virtual machine.
// It continuously fetches and executes instructions until it sees a HLT (halt).
void VM::execute() {
    try {
        std::cout << "Starting VM Execution...\n";

        // Infinite loop to run instructions one after another
        while (true) {
            Instruction instr = fetchNextInstruction(); // Fetch next instruction from memory
            executeInstruction(instr); // Decode and execute that instruction

            // If the instruction is HLT (halt), stop the execution
            if (instr.op == Opcode::HLT) {
                std::cout << "Program Halted.\n";
                break;
            }
        }
    } catch (const std::exception& ex) {
        // Handle any runtime errors and print the error message
        handleError(ex.what());
    }
}

// ---------------------------------------------------------------------------
// Function: fetchNextInstruction
// Purpose: Reads the next instruction from memory and decodes it.
Instruction VM::fetchNextInstruction() {
    uint16_t ip = cpu.r.ip; // Get the current instruction pointer

    // Read the opcode from memory at IP (Instruction Pointer)
    Opcode op = static_cast<Opcode>(memory[ip]);

    // Get how many bytes this instruction occupies
    uint8_t size = getInstructionSize(op);

    // Create a new Instruction object and set the opcode
    Instruction instr;
    instr.op = op;

    // Read the first operand if instruction size >= 2
    if (size >= 2) {
        instr.a1 = memory[ip + 1] | (memory[ip + 2] << 8); // Combine two bytes into a 16-bit value
    }

    // Read the second operand if size == 5 (not used in this current instruction set, but future-proof)
    if (size == 5) {
        instr.a2 = memory[ip + 3] | (memory[ip + 4] << 8);
    }

    cpu.r.ip += size; // Move the instruction pointer forward
    return instr;     // Return the decoded instruction
}

// ---------------------------------------------------------------------------
// Function: executeInstruction
// Purpose: Executes a decoded instruction by modifying registers, memory, or flags
void VM::executeInstruction(const Instruction& instr) {
    switch (instr.op) {
        case Opcode::NOP:
            // NOP (No Operation): Do nothing
            break;

        case Opcode::HLT:
            // HLT (Halt): Stop program execution and print state
            std::cout << "System Halted\n";
            std::cout << "AX: " << cpu.r.ax << ", BX: " << cpu.r.bx
                      << ", CX: " << cpu.r.cx << ", DX: " << cpu.r.dx
                      << ", SP: " << cpu.r.sp << "\n";

            // Print the last 32 bytes of stack memory (top of memory)
            RohitUtils::printhex(memory.raw() + 0xffff - 32, 32, ' ');
            break;

        // ----------- MOV Instructions -----------
        case Opcode::MOV:
            cpu.r.ax = instr.a1;  // Move value into AX register
            break;
        case Opcode::MOV_BX:
            cpu.r.bx = instr.a1;  // Move value into BX register
            break;
        case Opcode::MOV_CX:
            cpu.r.cx = instr.a1;  // Move value into CX register
            break;
        case Opcode::MOV_DX:
            cpu.r.dx = instr.a1;  // Move value into DX register
            break;
        case Opcode::MOV_SP:
            cpu.r.sp = instr.a1;  // Set the Stack Pointer to a specific location
            break;

        // ----------- Arithmetic Instructions -----------
        case Opcode::ADD:
            cpu.r.ax += cpu.r.bx;  // AX = AX + BX
            break;
        case Opcode::SUB:
            cpu.r.ax -= cpu.r.bx;  // AX = AX - BX
            break;
        case Opcode::MUL:
            cpu.r.ax *= cpu.r.bx;  // AX = AX * BX
            break;
        case Opcode::DIV:
            if (cpu.r.bx == 0) handleError("Division by zero"); // Prevent division by zero
            cpu.r.ax /= cpu.r.bx;  // AX = AX / BX
            break;

        // ----------- Flag Set/Clear Instructions -----------
        case Opcode::STE: cpu.setEqual(true); break;    // Set Equal flag
        case Opcode::CLE: cpu.setEqual(false); break;   // Clear Equal flag
        case Opcode::STG: cpu.setGreater(true); break;  // Set Greater flag
        case Opcode::CLG: cpu.setGreater(false); break; // Clear Greater flag
        case Opcode::STH: cpu.setHigher(true); break;   // Set High-bit flag
        case Opcode::CLH: cpu.setHigher(false); break;  // Clear High-bit flag
        case Opcode::STL: cpu.setLower(true); break;    // Set Lower flag
        case Opcode::CLL: cpu.setLower(false); break;   // Clear Lower flag

        // ----------- Stack Instructions -----------
        case Opcode::PUSH:
            // PUSH: Push the value of a register onto the stack
            switch (instr.a1) {
                case 0x00: push(cpu.r.ax); break;
                case 0x01: push(cpu.r.bx); break;
                case 0x02: push(cpu.r.cx); break;
                case 0x03: push(cpu.r.dx); break;
                default: handleError("Invalid register for PUSH"); break;
            }
            break;

        case Opcode::POP:
            // POP: Pop value from stack into a register
            switch (instr.a1) {
                case 0x00: cpu.r.ax = pop(); break;
                case 0x01: cpu.r.bx = pop(); break;
                case 0x02: cpu.r.cx = pop(); break;
                case 0x03: cpu.r.dx = pop(); break;
                default: handleError("Invalid register for POP"); break;
            }
            break;

        default:
            // If an unknown instruction is found
            handleError("Illegal Instruction");
            break;
    }
}

// ---------------------------------------------------------------------------
// Function: push
// Purpose: Push a 16-bit value onto the stack
// Stack grows downward in memory. SP (Stack Pointer) is decremented.
void VM::push(uint16_t val) {
    if (cpu.r.sp < 2) handleError("Stack Overflow");  // Prevent writing before memory start

    cpu.r.sp -= 2;                    // Make space for 2 bytes
    memory[cpu.r.sp] = val & 0xff;    // Store low byte
    memory[cpu.r.sp + 1] = (val >> 8) & 0xff; // Store high byte
}

// ---------------------------------------------------------------------------
// Function: pop
// Purpose: Pop a 16-bit value from the stack
// Stack grows downward, so popping means reading and then incrementing SP.
uint16_t VM::pop() {
    if (cpu.r.sp > Memory::SIZE - 2) handleError("Stack Underflow"); // Prevent reading invalid memory

    // Read two bytes from stack and combine into one 16-bit value
    uint16_t val = memory[cpu.r.sp] | (memory[cpu.r.sp + 1] << 8);
    cpu.r.sp += 2; // Move SP up (free the popped space)
    return val;
}

// ---------------------------------------------------------------------------
// Function: loadProgram
// Purpose: Loads a program (set of instructions) into VM memory starting from address 0
void VM::loadProgram(const std::vector<Instruction>& program) {
    uint8_t* mem = memory.raw();  // Get raw pointer to memory array

    for (const auto& instr : program) {
        // Store the opcode
        mem[breakLine++] = static_cast<uint8_t>(instr.op);

        // Store the first operand if applicable
        uint8_t size = getInstructionSize(instr.op);
        if (size >= 2) {
            mem[breakLine++] = instr.a1 & 0xff;
            mem[breakLine++] = (instr.a1 >> 8) & 0xff;
        }

        // Store the second operand (only used if instruction needs 5 bytes)
        if (size == 5) {
            mem[breakLine++] = instr.a2 & 0xff;
            mem[breakLine++] = (instr.a2 >> 8) & 0xff;
        }
    }
}

// ---------------------------------------------------------------------------
// Function: getInstructionSize
// Purpose: Returns how many bytes each instruction takes in memory
// Important because different instructions have different sizes
uint8_t VM::getInstructionSize(Opcode op) {
    static std::map<Opcode, uint8_t> sizeMap = {
        {Opcode::NOP, 1}, {Opcode::HLT, 1},
        {Opcode::MOV, 3}, {Opcode::MOV_BX, 3}, {Opcode::MOV_CX, 3},
        {Opcode::MOV_DX, 3}, {Opcode::MOV_SP, 3},
        {Opcode::STE, 1}, {Opcode::CLE, 1},
        {Opcode::STG, 1}, {Opcode::CLG, 1},
        {Opcode::STH, 1}, {Opcode::CLH, 1},
        {Opcode::STL, 1}, {Opcode::CLL, 1},
        {Opcode::PUSH, 3}, {Opcode::POP, 3},
        {Opcode::ADD, 1}, {Opcode::SUB, 1}, {Opcode::MUL, 1}, {Opcode::DIV, 1}
    };
    return sizeMap[op];
}

// ---------------------------------------------------------------------------
// Function: handleError
// Purpose: Prints an error message. If 'fatal' is true, the VM exits the program.
void VM::handleError(const std::string& msg, bool fatal) {
    std::cerr << "VM Error: " << msg << std::endl;
    if (fatal) std::exit(EXIT_FAILURE); // Exit the program on fatal error
}
