// Level 2: Atomic Operations - Weak CAS
// Tests that weak CAS can spuriously fail but eventually succeeds

#include <atomic>
#include <cstdio>

int main() {
    std::atomic<int> val(100);
    
    // Weak CAS may spuriously fail, so we try in a loop
    int attempts = 0;
    const int max_attempts = 1000;
    bool succeeded = false;
    
    while (attempts < max_attempts) {
        int expected = 100;
        if (val.compare_exchange_weak(expected, 200)) {
            succeeded = true;
            break;
        }
        // If it failed, expected should still be 100 (the actual value)
        // unless we had a spurious failure
        if (expected != 100) {
            // Real failure - value was changed by someone else (shouldn't happen in single thread)
            printf("[FAIL] test_cas_weak: unexpected value %d\n", expected);
            return 1;
        }
        attempts++;
    }
    
    if (!succeeded) {
        printf("[FAIL] test_cas_weak: too many spurious failures (%d)\n", max_attempts);
        return 1;
    }
    
    if (val.load() != 200) {
        printf("[FAIL] test_cas_weak: value not updated after success\n");
        return 1;
    }
    
    printf("[PASS] test_cas_weak (attempts=%d)\n", attempts + 1);
    return 0;
}
