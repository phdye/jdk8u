// Level 3: Assembly Stubs - SpinPause
// Tests SpinPause (CPU pause hint) from cygwin_x86_64.s

#include <cstdio>
#include <cstdint>
#include <time.h>

extern "C" {
    void SpinPause();
}

int main() {
    // SpinPause should not crash
    SpinPause();
    
    // Call it many times - should complete quickly
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    const int iterations = 1000000;
    for (int i = 0; i < iterations; i++) {
        SpinPause();
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000L +
                      (end.tv_nsec - start.tv_nsec);
    
    // Should complete in reasonable time (< 1 second for 1M iterations)
    if (elapsed_ns > 1000000000L) {
        printf("[FAIL] test_spin_pause: took too long: %ld ns\n", elapsed_ns);
        return 1;
    }
    
    // Each pause should be very short (< 1us average)
    long avg_ns = elapsed_ns / iterations;
    if (avg_ns > 1000) {
        printf("[WARN] test_spin_pause: avg %ld ns per pause (expected < 1000 ns)\n", avg_ns);
    }
    
    printf("[PASS] test_spin_pause (avg=%ld ns)\n", avg_ns);
    return 0;
}
