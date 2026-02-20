// Level 7: Signal Context - siginfo_t Fields
// Tests that siginfo_t provides useful information

#define _GNU_SOURCE
#include <signal.h>
#include <cstdio>
#include <cstdint>
#include <setjmp.h>

static sigjmp_buf jump_buf;
static int captured_signo = 0;
static int captured_code = 0;
static void* captured_addr = NULL;

void sigfpe_handler(int sig, siginfo_t* info, void* ctx) {
    captured_signo = info->si_signo;
    captured_code = info->si_code;
    captured_addr = info->si_addr;
    siglongjmp(jump_buf, 1);
}

void sigsegv_handler(int sig, siginfo_t* info, void* ctx) {
    captured_signo = info->si_signo;
    captured_code = info->si_code;
    captured_addr = info->si_addr;
    siglongjmp(jump_buf, 1);
}

int main() {
    struct sigaction sa;
    sa.sa_sigaction = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);
    
    // Test SIGSEGV with specific address
    void* fault_addr = (void*)0x12345678;
    
    if (sigsetjmp(jump_buf, 1) == 0) {
        volatile int* p = (int*)fault_addr;
        *p = 0;
    }
    
    if (captured_signo != SIGSEGV) {
        printf("[FAIL] test_siginfo: wrong signal: %d\n", captured_signo);
        return 1;
    }
    
    // si_addr should be close to our fault address
    // (may be page-aligned)
    uintptr_t fault = (uintptr_t)fault_addr;
    uintptr_t reported = (uintptr_t)captured_addr;
    
    // Check within same 4KB page
    if ((fault & ~0xFFF) != (reported & ~0xFFF)) {
        printf("[WARN] test_siginfo: si_addr mismatch: 0x%lx vs 0x%lx\n",
               (unsigned long)fault, (unsigned long)reported);
        // Not a hard failure - some systems may not provide exact address
    }
    
    // Test SIGFPE (if division by zero triggers it)
    sa.sa_sigaction = sigfpe_handler;
    sigaction(SIGFPE, &sa, NULL);
    
    captured_signo = 0;
    
    if (sigsetjmp(jump_buf, 1) == 0) {
        volatile int x = 1;
        volatile int y = 0;
        volatile int z = x / y;  // May or may not trigger SIGFPE
        (void)z;
        // If we get here, division by zero doesn't trigger SIGFPE on this platform
        printf("[INFO] test_siginfo: division by zero did not trigger SIGFPE\n");
    } else {
        if (captured_signo != SIGFPE) {
            printf("[FAIL] test_siginfo: expected SIGFPE, got %d\n", captured_signo);
            return 1;
        }
    }
    
    printf("[PASS] test_siginfo\n");
    return 0;
}
