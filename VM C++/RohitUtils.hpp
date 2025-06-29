// RohitUtils.hpp
#pragma once  // This ensures the header file is included only once during compilation (prevents duplicate definitions)

// Include standard headers for fixed-width integers and functions
#include <cstdint>     // For fixed-width integer types like uint8_t, uint16_t
#include <string>      // For using string types (if needed in extensions)
#include <cstdio>      // For printf and related functions
#include <cstring>     // For basic string/memory manipulation functions
#include <arpa/inet.h> // For IP address structures and network-to-host conversions

// ==========================================================
// Author: Rohit Yadav
// Purpose of this File:
//   - This header file declares **helper utility functions** used in the VM project.
//   - It also defines custom type aliases (like int8, int16) for consistency.
//   - The functions declared here are implemented in RohitUtils.cpp.
// ==========================================================


// ----------------------
// Type Aliases
// ----------------------
// These typedef-style aliases make the code easier to read
// and match typical system-level variable naming conventions.

using int8  = uint8_t;   // int8: 8-bit unsigned integer (0–255)
using int16 = uint16_t;  // int16: 16-bit unsigned integer (0–65535)
using int32 = uint32_t;  // int32: 32-bit unsigned integer

// ----------------------
// Namespace RohitUtils
// ----------------------
// All utility functions are wrapped inside this namespace
// to avoid naming collisions with other parts of the project
// or with standard library functions.
namespace RohitUtils {

    // -------------------------------------------------------------------
    // Function: copy
    // Description:
    //   - Copies a block of memory from source (src) to destination (dst)
    //   - Similar to the standard `memcpy()` but manually written
    // Parameters:
    //   - dst: pointer to the destination memory block
    //   - src: pointer to the source memory block
    //   - size: number of bytes to copy
    // Why it's useful:
    //   - Used in VM to copy values between memory regions (e.g., during stack operations)
    void copy(int8* dst, const int8* src, int16 size);

    // -------------------------------------------------------------------
    // Function: nstoh (Network Short to Host)
    // Description:
    //   - Converts a 16-bit value from **network byte order** (big-endian)
    //     to **host byte order** (depends on CPU architecture, often little-endian)
    // Parameters:
    //   - srcport: 16-bit port number in network byte order
    // Returns:
    //   - Converted port number in host format
    // Why it's useful:
    //   - Ensures portability and correctness when reading/writing binary data
    //     especially if the VM is extended to interact with networks.
    int16 nstoh(int16 srcport);

    // -------------------------------------------------------------------
    // Function: zero
    // Description:
    //   - Sets a block of memory to all zeros (0)
    // Parameters:
    //   - str: pointer to the memory block
    //   - size: number of bytes to clear
    // Why it's useful:
    //   - Used to initialize or reset parts of memory (RAM, stack, buffers)
    //   - Prevents bugs caused by uninitialized memory.
    void zero(int8* str, int16 size);

    // -------------------------------------------------------------------
    // Function: printhex
    // Description:
    //   - Prints a memory block as hexadecimal values
    //   - Allows you to see what's inside RAM or registers in a readable format
    // Parameters:
    //   - str: pointer to the memory block
    //   - size: number of bytes to print
    //   - delim: optional character to separate hex values (like space, dash, etc.)
    // Why it's useful:
    //   - Essential for debugging the VM or viewing the memory contents visually.
    void printhex(const int8* str, int16 size, int8 delim = 0);

    // -------------------------------------------------------------------
    // Function: todotted
    // Description:
    //   - Converts a 32-bit IP address into human-readable dotted-decimal format (e.g., "192.168.1.1")
    // Parameters:
    //   - ip: 32-bit IP address (in_addr_t)
    // Returns:
    //   - A pointer to a string that contains the converted IP
    // Why it's useful:
    //   - Helpful if the VM is extended to simulate networking features.
    const char* todotted(in_addr_t ip);

} // namespace RohitUtils
