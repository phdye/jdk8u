// Level 4: OS Abstraction - Clock and Timing
// Tests clock_gettime, gettimeofday, and timing accuracy

#define _GNU_SOURCE
#include <time.h>
#include <sys/time.h>
#include <cstdio>
#include <unistd.h>

int main() {
    struct timespec ts;
    
    // Test CLOCK_REALTIME
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
        printf("[FAIL] test_timing: CLOCK_REALTIME failed\n");
        return 1;
    }
    
    // Should be after year 2020 (1577836800 = Jan 1, 2020)
    if (ts.tv_sec < 1577836800) {
        printf("[FAIL] test_timing: CLOCK_REALTIME value unreasonable: %ld\n",
               (long)ts.tv_sec);
        return 1;
    }
    
    // Test CLOCK_MONOTONIC
    struct timespec mono1, mono2;
    if (clock_gettime(CLOCK_MONOTONIC, &mono1) != 0) {
        printf("[FAIL] test_timing: CLOCK_MONOTONIC failed\n");
        return 1;
    }
    
    // Sleep a bit
    usleep(50000);  // 50ms
    
    if (clock_gettime(CLOCK_MONOTONIC, &mono2) != 0) {
        printf("[FAIL] test_timing: CLOCK_MONOTONIC second call failed\n");
        return 1;
    }
    
    long elapsed_ns = (mono2.tv_sec - mono1.tv_sec) * 1000000000L +
                      (mono2.tv_nsec - mono1.tv_nsec);
    
    // Should be approximately 50ms (allow 20ms to 200ms)
    if (elapsed_ns < 20000000 || elapsed_ns > 200000000) {
        printf("[FAIL] test_timing: elapsed %ld ns, expected ~50000000 ns\n", elapsed_ns);
        return 1;
    }
    
    // Test monotonic is actually monotonic
    for (int i = 0; i < 100; i++) {
        clock_gettime(CLOCK_MONOTONIC, &mono1);
        clock_gettime(CLOCK_MONOTONIC, &mono2);
        
        if (mono2.tv_sec < mono1.tv_sec ||
            (mono2.tv_sec == mono1.tv_sec && mono2.tv_nsec < mono1.tv_nsec)) {
            printf("[FAIL] test_timing: CLOCK_MONOTONIC went backwards\n");
            return 1;
        }
    }
    
    // Test gettimeofday
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0) {
        printf("[FAIL] test_timing: gettimeofday failed\n");
        return 1;
    }
    
    if (tv.tv_sec < 1577836800) {
        printf("[FAIL] test_timing: gettimeofday value unreasonable\n");
        return 1;
    }
    
    // Test clock() for CPU time
    clock_t c1 = clock();
    
    // Do some busy work
    volatile long sum = 0;
    for (long i = 0; i < 10000000; i++) {
        sum += i;
    }
    
    clock_t c2 = clock();
    
    if (c2 <= c1) {
        printf("[WARN] test_timing: clock() didn't advance (c1=%ld, c2=%ld)\n",
               (long)c1, (long)c2);
        // Not a hard failure - clock() may have low resolution
    }
    
    printf("[PASS] test_timing (elapsed=%ld ns)\n", elapsed_ns);
    return 0;
}
