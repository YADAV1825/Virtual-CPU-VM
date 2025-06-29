# ⚙️ Virtual-CPU + Basic VM - IN LINUX

Built by : **Rohit Yadav**  
*NIT Jalandhar*

---

## 🧾 Short Summary

Built a 16-bit virtual CPU from scratch in C++ with simulated 65KB of RAM.  
It is **not just a virtual CPU** — this project also functions as a **basic Virtual Machine (VM)** capable of executing instruction-based programs.

---

# ⚙️ RohitVM — A 16-bit Virtual CPU + Virtual Machine in C++

![badge](https://img.shields.io/badge/status-Working-brightgreen)  
🎓 Made from scratch by [Rohit Yadav](https://github.com/YourGitHubUsername) — *pure C++, zero dependencies.*

---

## 🚀 About the Project

**RohitVM** is a lightweight, hand-crafted **16-bit Virtual Machine and CPU simulator** written entirely in C++.  
It mimics the basic functionality of a real CPU — registers, memory, instructions — and executes full programs line-by-line via a custom instruction set.

This project is designed for **learning**, **experimentation**, and **educational demos**.  
Feel free to fork it, modify it, or build tools on top of it!

---

![image](https://github.com/user-attachments/assets/31340bde-b08a-46b0-a677-36c5240fb175)
![image](https://github.com/user-attachments/assets/b2b431b0-9131-4b46-a5b9-4dde33ac5243)
![image](https://github.com/user-attachments/assets/83ce8a82-4b58-4aaf-bf68-39674171b9aa)

---

## 🧠 Core Features

✅ **16-bit architecture** — all operations on 16-bit unsigned integers  
✅ **65 KB virtual RAM** — simulated with 256 memory slots (uint16_t)  
✅ **8 general-purpose registers** — R1 to R8  
✅ **Virtual Instruction Execution** — a full interpreter cycle  
✅ **Custom instruction set** including:

| Opcode   | Description                             |
|----------|-----------------------------------------|
| `MOV`    | Move value/register into another register |
| `ADD`    | Add values and store in a register       |
| `SUB`    | Subtract values                          |
| `MUL`    | Multiply values                          |
| `DIV`    | Divide values (integer division)         |
| `JMP`    | Jump to a specific instruction address   |
| `CMP`    | Compare two values                       |
| `JE`     | Jump if equal                            |
| `JNE`    | Jump if not equal                        |
| `JG`     | Jump if greater                          |
| `JL`     | Jump if less                             |
| `PRINT`  | Print a register or immediate value      |
| `HLT`    | Halt execution                           |

---

## 🛠️ How It Works

Each instruction is written in a simple, assembly-like syntax:

```
MOV R1, 10
MOV R2, 20
ADD R1, R2
PRINT R1
HLT
```

The VM will:
- Parse and load this program
- Execute instructions line-by-line
- Output results using `std::cout`

---

## 🧪 Example Output

```
Starting VM Execution...
30
Program Halted.
```

---

## 📁 Project Structure

```
📦 RohitVM/
├── main.cpp           → Entry point, test program
├── RohitVM.hpp        → Class & struct declarations
├── RohitVM.cpp        → CPU + VM execution logic
├── RohitUtils.hpp     → Utility function declarations
├── RohitUtils.cpp     → Utility function implementations
```

---

## 🧩 Build & Run

### 📦 Compile with g++:

```bash
g++ -std=c++17 main.cpp RohitVM.cpp RohitUtils.cpp -o VirtualCPU
```

### ▶️ Run:

```bash
./VirtualCPU
```

> 💡 Requires: g++ 9+ and any modern Linux/Unix/Mac system

---

## 💡 Use Cases

- 🧑‍🎓 Teaching students how CPUs work
- 🔬 Experimenting with instruction set design
- 🧪 Learning virtual machine and runtime concepts in C++
- 🧱 Foundation for building higher-level virtual languages or emulators

---

## 📌 To-Do / Ideas  
> 🧠 **It is a Virtual Machine already** — but here’s how we can make it even better:

- [ ] Implement stack + PUSH/POP
- [ ] Add function calls (CALL/RET)
- [ ] Create an assembly file loader
- [ ] Write a Bytecode/Assembler
- [ ] Create a Mini High-Level Language
- [ ] Add Debugger or CLI Shell
- [ ] Multi-threading or Scheduling
- [ ] Add GUI/Web visualizer
- [ ] Extend memory to full 65,536 words

---

## 🙏 Acknowledgements

This project was built by me completely from scratch using only the C++ Standard Library.  
Thanks for checking it out!

---

## 📜 License

MIT License — use it freely in open/closed source projects.

---

**Give it a ⭐ if you like it, and feel free to fork!**
