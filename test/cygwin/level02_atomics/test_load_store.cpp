// Level 2: Atomic Operations - Basic Load/Store
// Tests atomic load/store with various memory orderings

#include <atomic>
#include <cstdio>
#include <cstdint>

int main() {
    // Test all integer sizes
    std::atomic<int8_t> a8(0);
    std::atomic<int16_t> a16(0);
    std::atomic<int32_t> a32(0);
    std::atomic<int64_t> a64(0);
    std::atomic<void*> aptr(nullptr);
    
    // Relaxed ordering
    a8.store(42, std::memory_order_relaxed);
    a16.store(1234, std::memory_order_relaxed);
    a32.store(12345678, std::memory_order_relaxed);
    a64.store(1234567890123456LL, std::memory_order_relaxed);
    aptr.store((void*)0xDEADBEEF, std::memory_order_relaxed);
    
    if (a8.load(std::memory_order_relaxed) != 42) {
        printf("[FAIL] test_load_store: int8 relaxed\n");
        return 1;
    }
    if (a16.load(std::memory_order_relaxed) != 1234) {
        printf("[FAIL] test_load_store: int16 relaxed\n");
        return 1;
    }
    if (a32.load(std::memory_order_relaxed) != 12345678) {
        printf("[FAIL] test_load_store: int32 relaxed\n");
        return 1;
    }
    if (a64.load(std::memory_order_relaxed) != 1234567890123456LL) {
        printf("[FAIL] test_load_store: int64 relaxed\n");
        return 1;
    }
    if (aptr.load(std::memory_order_relaxed) != (void*)0xDEADBEEF) {
        printf("[FAIL] test_load_store: pointer relaxed\n");
        return 1;
    }
    
    // Acquire/Release ordering
    a64.store(999, std::memory_order_release);
    int64_t v = a64.load(std::memory_order_acquire);
    if (v != 999) {
        printf("[FAIL] test_load_store: acquire/release\n");
        return 1;
    }
    
    // Sequential consistency
    a32.store(777, std::memory_order_seq_cst);
    if (a32.load(std::memory_order_seq_cst) != 777) {
        printf("[FAIL] test_load_store: seq_cst\n");
        return 1;
    }
    
    printf("[PASS] test_load_store\n");
    return 0;
}
