// Level 1: Cygwin Compatibility - Resource Limits
// Tests that RLIMIT constants and getrlimit() work

#include <sys/resource.h>
#include <cstdio>
#include <cerrno>

#ifdef __CYGWIN__
#include "cygwin_compat.hpp"
#endif

int main() {
    struct rlimit rl;
    
    // Required RLIMIT constants that JVM uses
    int limits[] = {
        RLIMIT_STACK,   // Stack size - critical for thread creation
        RLIMIT_NOFILE,  // Open files - critical for I/O
        RLIMIT_AS,      // Address space - heap limits
        RLIMIT_CORE,    // Core dump size
        RLIMIT_CPU,     // CPU time
        RLIMIT_DATA,    // Data segment
        RLIMIT_FSIZE,   // File size
    };
    const char* names[] = {
        "RLIMIT_STACK", "RLIMIT_NOFILE", "RLIMIT_AS", "RLIMIT_CORE",
        "RLIMIT_CPU", "RLIMIT_DATA", "RLIMIT_FSIZE"
    };
    
    for (size_t i = 0; i < sizeof(limits)/sizeof(limits[0]); i++) {
        if (getrlimit(limits[i], &rl) != 0) {
            // Some limits may not be fully supported on Cygwin
            // but the call shouldn't fail
            printf("[WARN] test_rlimits: getrlimit(%s) failed: %d\n", names[i], errno);
            continue;
        }
    }
    
    // Test that we can get stack limit (critical for JVM)
    if (getrlimit(RLIMIT_STACK, &rl) != 0) {
        printf("[FAIL] test_rlimits: RLIMIT_STACK not available\n");
        return 1;
    }
    
    // Stack limit should be reasonable (at least 64KB)
    if (rl.rlim_cur < 64 * 1024 && rl.rlim_cur != RLIM_INFINITY) {
        printf("[FAIL] test_rlimits: RLIMIT_STACK too small: %lu\n", 
               (unsigned long)rl.rlim_cur);
        return 1;
    }
    
    // Test RLIM_INFINITY
    if (RLIM_INFINITY == 0) {
        printf("[FAIL] test_rlimits: RLIM_INFINITY is zero\n");
        return 1;
    }
    
    printf("[PASS] test_rlimits\n");
    return 0;
}
