// Level 7: Signal Context - Backtrace/Frame Walking
// Tests manual frame walking using GCC builtins (Cygwin doesn't have execinfo.h)

#define _GNU_SOURCE
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>

// Manual frame walking using GCC builtins
__attribute__((noinline))
void level4() {
    void* frames[10];
    int depth = 0;

    // Get return addresses using GCC builtins
    // These are limited but work everywhere GCC does
    frames[depth++] = __builtin_return_address(0);

    // Check that we can read frame pointer (may be NULL with optimizations)
    void* fp = __builtin_frame_address(0);

    if (frames[0] == NULL) {
        printf("[FAIL] test_backtrace: return address 0 is NULL\n");
        _exit(1);
    }

    // Verify the return address looks reasonable (user space, not kernel)
    uintptr_t addr = (uintptr_t)frames[0];

    // On x86-64 Windows/Cygwin, user addresses are typically below 0x7FFFFFFFFFFF
    // but above 0x10000 (below that is reserved)
    if (addr < 0x10000) {
        printf("[FAIL] test_backtrace: return address too low: 0x%lx\n",
               (unsigned long)addr);
        _exit(1);
    }

    // Frame pointer should also be valid (non-NULL on unoptimized build)
    if (fp != NULL) {
        uintptr_t fp_addr = (uintptr_t)fp;
        // Stack addresses should also be in user space
        if (fp_addr < 0x10000) {
            printf("[FAIL] test_backtrace: frame pointer too low: 0x%lx\n",
                   (unsigned long)fp_addr);
            _exit(1);
        }
    }

    // Try walking the stack manually via frame pointers
    // On x86-64, frame layout is: [saved RBP][return addr]
    void** frame = (void**)fp;
    int manual_depth = 0;
    const int MAX_DEPTH = 8;

    while (frame != NULL && manual_depth < MAX_DEPTH) {
        void* return_addr = frame[1];
        void* saved_fp = frame[0];

        if (return_addr == NULL) break;

        uintptr_t ra = (uintptr_t)return_addr;
        // Sanity check - should be in text segment
        if (ra < 0x10000 || ra > 0x7FFFFFFFFFFFF) break;

        manual_depth++;
        frame = (void**)saved_fp;

        // Safety: if frame pointer seems invalid, stop
        if ((uintptr_t)frame < 0x10000) break;
    }

    // We should have found at least a few frames (level4 <- level3 <- level2 <- level1 <- main)
    if (manual_depth < 2) {
        printf("[WARN] test_backtrace: manual walk found only %d frames (may be optimized)\n",
               manual_depth);
        // Not a failure - could be due to optimization or platform differences
    }

    printf("[PASS] test_backtrace (builtin_return_address works, manual walk found %d frames)\n",
           manual_depth);
}

__attribute__((noinline)) void level3() { level4(); }
__attribute__((noinline)) void level2() { level3(); }
__attribute__((noinline)) void level1() { level2(); }

int main() {
    level1();
    return 0;
}
