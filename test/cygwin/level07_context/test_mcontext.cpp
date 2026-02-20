// Level 7: Signal Context - mcontext_t Register Access
// Tests that we can extract registers from signal context

#define _GNU_SOURCE
#include <signal.h>
#include <ucontext.h>
#include <cstdio>
#include <cstdint>
#include <setjmp.h>

static sigjmp_buf jump_buf;
static uintptr_t captured_rip = 0;
static uintptr_t captured_rsp = 0;
static uintptr_t captured_rbp = 0;

void sigsegv_handler(int sig, siginfo_t* info, void* ctx) {
    ucontext_t* uc = (ucontext_t*)ctx;
    mcontext_t* mc = &uc->uc_mcontext;
    
#ifdef __x86_64__
    captured_rip = mc->rip;
    captured_rsp = mc->rsp;
    captured_rbp = mc->rbp;
#else
    captured_rip = mc->eip;
    captured_rsp = mc->esp;
    captured_rbp = mc->ebp;
#endif
    
    siglongjmp(jump_buf, 1);
}

int main() {
    struct sigaction sa;
    sa.sa_sigaction = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGSEGV, &sa, NULL) != 0) {
        printf("[FAIL] test_mcontext: sigaction failed\n");
        return 1;
    }
    
    if (sigsetjmp(jump_buf, 1) == 0) {
        // Trigger SIGSEGV
        volatile int* bad_ptr = (int*)0x1;
        *bad_ptr = 42;
        
        printf("[FAIL] test_mcontext: SIGSEGV not delivered\n");
        return 1;
    }
    
    // Verify we captured reasonable values
    if (captured_rip == 0) {
        printf("[FAIL] test_mcontext: RIP is 0\n");
        return 1;
    }
    
    if (captured_rsp == 0) {
        printf("[FAIL] test_mcontext: RSP is 0\n");
        return 1;
    }
    
    // RIP should be in user space (not kernel)
    // On 64-bit, user space is typically below 0x7FFFFFFFFFFF
    if (captured_rip > 0x7FFFFFFFFFFFF) {
        printf("[FAIL] test_mcontext: RIP looks like kernel address\n");
        return 1;
    }
    
    // RSP should be reasonable stack address
    if (captured_rsp > 0x7FFFFFFFFFFFF) {
        printf("[FAIL] test_mcontext: RSP looks like kernel address\n");
        return 1;
    }
    
    printf("[PASS] test_mcontext (RIP=0x%lx, RSP=0x%lx, RBP=0x%lx)\n",
           (unsigned long)captured_rip, (unsigned long)captured_rsp,
           (unsigned long)captured_rbp);
    return 0;
}
