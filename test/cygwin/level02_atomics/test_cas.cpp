// Level 2: Atomic Operations - Compare-and-Swap
// Tests CAS operations for all widths

#include <atomic>
#include <cstdio>
#include <cstdint>

template<typename T>
bool test_cas_success(const char* name, T init, T newval) {
    std::atomic<T> val(init);
    T expected = init;
    
    bool success = val.compare_exchange_strong(expected, newval);
    if (!success) {
        printf("[FAIL] %s: CAS should succeed\n", name);
        return false;
    }
    if (val.load() != newval) {
        printf("[FAIL] %s: value not updated\n", name);
        return false;
    }
    return true;
}

template<typename T>
bool test_cas_failure(const char* name, T init, T wrong_expected, T newval) {
    std::atomic<T> val(init);
    T expected = wrong_expected;
    
    bool success = val.compare_exchange_strong(expected, newval);
    if (success) {
        printf("[FAIL] %s: CAS should fail\n", name);
        return false;
    }
    // expected should be updated to actual value
    if (expected != init) {
        printf("[FAIL] %s: expected not updated on failure\n", name);
        return false;
    }
    // value should be unchanged
    if (val.load() != init) {
        printf("[FAIL] %s: value changed on failed CAS\n", name);
        return false;
    }
    return true;
}

int main() {
    // 8-bit CAS
    if (!test_cas_success<int8_t>("int8_success", 10, 20)) return 1;
    if (!test_cas_failure<int8_t>("int8_failure", 10, 99, 20)) return 1;
    
    // 16-bit CAS
    if (!test_cas_success<int16_t>("int16_success", 1000, 2000)) return 1;
    if (!test_cas_failure<int16_t>("int16_failure", 1000, 9999, 2000)) return 1;
    
    // 32-bit CAS
    if (!test_cas_success<int32_t>("int32_success", 100000, 200000)) return 1;
    if (!test_cas_failure<int32_t>("int32_failure", 100000, 999999, 200000)) return 1;
    
    // 64-bit CAS
    if (!test_cas_success<int64_t>("int64_success", 10000000000LL, 20000000000LL)) return 1;
    if (!test_cas_failure<int64_t>("int64_failure", 10000000000LL, 99999999999LL, 20000000000LL)) return 1;
    
    // Pointer CAS
    if (!test_cas_success<void*>("ptr_success", (void*)0x1000, (void*)0x2000)) return 1;
    if (!test_cas_failure<void*>("ptr_failure", (void*)0x1000, (void*)0x9999, (void*)0x2000)) return 1;
    
    printf("[PASS] test_cas\n");
    return 0;
}
