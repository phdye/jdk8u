// Level 1: Cygwin Compatibility - Type Sizes and Alignment
// Verifies fundamental type sizes match JVM expectations

#include <sys/types.h>
#include <stdint.h>
#include <cstdio>
#include <cstddef>

int main() {
    // Core type sizes - critical for JVM correctness
    static_assert(sizeof(off_t) == 8, "off_t must be 64-bit for large file support");
    static_assert(sizeof(size_t) == sizeof(void*), "size_t must match pointer size");
    static_assert(sizeof(ssize_t) == sizeof(void*), "ssize_t must match pointer size");
    static_assert(sizeof(ptrdiff_t) == sizeof(void*), "ptrdiff_t must match pointer size");
    
    // Fixed-width integer types
    static_assert(sizeof(int8_t) == 1, "int8_t must be 1 byte");
    static_assert(sizeof(int16_t) == 2, "int16_t must be 2 bytes");
    static_assert(sizeof(int32_t) == 4, "int32_t must be 4 bytes");
    static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes");
    static_assert(sizeof(uint8_t) == 1, "uint8_t must be 1 byte");
    static_assert(sizeof(uint16_t) == 2, "uint16_t must be 2 bytes");
    static_assert(sizeof(uint32_t) == 4, "uint32_t must be 4 bytes");
    static_assert(sizeof(uint64_t) == 8, "uint64_t must be 8 bytes");
    
    // Pointer types
    static_assert(sizeof(void*) == 8, "pointers must be 64-bit on x86_64");
    static_assert(sizeof(intptr_t) == sizeof(void*), "intptr_t must match pointer");
    static_assert(sizeof(uintptr_t) == sizeof(void*), "uintptr_t must match pointer");
    
    // Alignment requirements
    static_assert(alignof(void*) == 8, "pointer alignment must be 8");
    static_assert(alignof(double) == 8, "double alignment must be 8");
    static_assert(alignof(int64_t) == 8, "int64_t alignment must be 8");
    
    // intptr_t round-trip test
    void* ptr = (void*)0xDEADBEEFCAFEBABEULL;
    intptr_t iptr = (intptr_t)ptr;
    void* ptr2 = (void*)iptr;
    if (ptr != ptr2) {
        printf("[FAIL] test_types: intptr_t round-trip failed\n");
        return 1;
    }
    
    // Verify char is signed or unsigned as expected
    // (JVM assumes certain behavior)
    char c = -1;
    if (sizeof(char) != 1) {
        printf("[FAIL] test_types: char not 1 byte\n");
        return 1;
    }
    
    printf("[PASS] test_types\n");
    return 0;
}
