// RohitUtils.cpp
// This file contains utility/helper functions used across the virtual machine (VM) project.
// These functions handle tasks like memory copying, byte order conversion, zeroing memory,
// printing memory contents in hex format, and converting IP addresses to readable form.

#include "RohitUtils.hpp"

// All utility functions are defined inside the RohitUtils namespace
namespace RohitUtils {

    // -------------------------------
    // Function: copy
    // Purpose: Copies a block of memory from 'src' to 'dst'
    // Parameters:
    //   - dst: destination memory address
    //   - src: source memory address
    //   - size: number of bytes to copy
    // Why it's here:
    //   - Useful for copying memory (e.g., stack frames, data blocks) within the VM.
    //   - Acts like the standard memcpy function but defined manually here for control.
    // Helps the code by:
    //   - Avoiding dependency on external libraries.
    void copy(int8* dst, const int8* src, int16 size) {
        for (int16 i = 0; i < size; ++i)
            dst[i] = src[i];  // Copy byte-by-byte
    }

    // -------------------------------
    // Function: nstoh (Network Short to Host)
    // Purpose: Converts a 16-bit number from network byte order to host byte order
    // Parameters:
    //   - srcport: the 16-bit number (typically a port) in network byte order
    // Why it's here:
    //   - Different systems store numbers in different ways (Big Endian vs Little Endian).
    //   - Network protocols use Big Endian (most significant byte first),
    //     but CPUs might use Little Endian (least significant byte first).
    // Helps the code by:
    //   - Ensuring correct interpretation of numbers when dealing with networking or memory formats.
    int16 nstoh(int16 srcport) {
        // Separate high and low bytes, then swap them
        int8 a = (srcport & 0xff00) >> 8;  // Get high byte
        int8 b = srcport & 0x00ff;         // Get low byte
        return (b << 8) | a;               // Swap and combine
    }

    // -------------------------------
    // Function: zero
    // Purpose: Clears (sets to zero) a block of memory
    // Parameters:
    //   - str: pointer to the memory block
    //   - size: number of bytes to set to zero
    // Why it's here:
    //   - Useful to initialize memory (e.g., RAM, registers, buffers) before use.
    // Helps the code by:
    //   - Avoiding undefined behavior by making sure all values start from 0.
    void zero(int8* str, int16 size) {
        for (int16 i = 0; i < size; ++i)
            str[i] = 0;
    }

    // -------------------------------
    // Function: printhex
    // Purpose: Prints a memory block as hexadecimal numbers
    // Parameters:
    //   - str: pointer to the memory block
    //   - size: number of bytes to print
    //   - delim: optional character to print between hex bytes (e.g., space or dash)
    // Why it's here:
    //   - Helps in debugging by allowing developers to see the memory contents in readable hex.
    // Helps the code by:
    //   - Allowing the VM to show what's stored in memory in a clean and readable way.
    void printhex(const int8* str, int16 size, int8 delim) {
        for (int16 i = 0; i < size; ++i) {
            printf("%.02x", str[i]);  // Print each byte in 2-digit hex format
            if (delim)
                printf("%c", delim);  // Add delimiter between bytes, if specified
        }
        printf("\n");
        fflush(stdout);  // Force print output immediately (not buffered)
    }

    // -------------------------------
    // Function: todotted
    // Purpose: Converts a 32-bit IP address (in_addr_t) to a human-readable dotted decimal format
    //          Example: 3232235777 -> "192.168.1.1"
    // Parameters:
    //   - ip: IPv4 address as a 32-bit integer
    // Returns:
    //   - A string representing the IP in dotted-decimal format
    // Why it's here:
    //   - Some instructions or debug outputs may involve IP addresses.
    // Helps the code by:
    //   - Turning low-level binary IP into something human-readable.
    const char* todotted(in_addr_t ip) {
        static char buf[16];  // Static buffer to hold the result (enough for "xxx.xxx.xxx.xxx")
        snprintf(buf, sizeof(buf), "%d.%d.%d.%d",
            (ip & 0x000000FF),         // Extract lowest byte (1st part of IP)
            (ip & 0x0000FF00) >> 8,    // 2nd part
            (ip & 0x00FF0000) >> 16,   // 3rd part
            (ip & 0xFF000000) >> 24);  // 4th part
        return buf;
    }

} // namespace RohitUtils
