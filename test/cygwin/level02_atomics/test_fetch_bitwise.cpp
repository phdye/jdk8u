// Level 2: Atomic Operations - Bitwise Fetch Operations
// Tests fetch_or, fetch_and, fetch_xor

#include <atomic>
#include <cstdio>
#include <cstdint>

int main() {
    std::atomic<uint32_t> flags(0);
    uint32_t old;
    
    // fetch_or - set bits
    old = flags.fetch_or(0x0F);
    if (old != 0) {
        printf("[FAIL] test_fetch_bitwise: fetch_or old value\n");
        return 1;
    }
    if (flags.load() != 0x0F) {
        printf("[FAIL] test_fetch_bitwise: fetch_or new value\n");
        return 1;
    }
    
    // fetch_or more bits
    old = flags.fetch_or(0xF0);
    if (old != 0x0F) {
        printf("[FAIL] test_fetch_bitwise: fetch_or second old value\n");
        return 1;
    }
    if (flags.load() != 0xFF) {
        printf("[FAIL] test_fetch_bitwise: fetch_or second new value\n");
        return 1;
    }
    
    // fetch_and - clear bits
    old = flags.fetch_and(0x0F);
    if (old != 0xFF) {
        printf("[FAIL] test_fetch_bitwise: fetch_and old value\n");
        return 1;
    }
    if (flags.load() != 0x0F) {
        printf("[FAIL] test_fetch_bitwise: fetch_and new value\n");
        return 1;
    }
    
    // fetch_xor - toggle bits
    old = flags.fetch_xor(0xFF);
    if (old != 0x0F) {
        printf("[FAIL] test_fetch_bitwise: fetch_xor old value\n");
        return 1;
    }
    if (flags.load() != 0xF0) {
        printf("[FAIL] test_fetch_bitwise: fetch_xor new value\n");
        return 1;
    }
    
    // Test with 64-bit values
    std::atomic<uint64_t> flags64(0);
    flags64.fetch_or(0x0F0F0F0F0F0F0F0FULL);
    if (flags64.load() != 0x0F0F0F0F0F0F0F0FULL) {
        printf("[FAIL] test_fetch_bitwise: 64-bit fetch_or\n");
        return 1;
    }
    
    printf("[PASS] test_fetch_bitwise\n");
    return 0;
}
