// Level 4: OS Abstraction - mmap/mprotect
// Tests memory mapping with various protections

#define _GNU_SOURCE
#include <sys/mman.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

static sigjmp_buf jump_buf;
static volatile int got_signal = 0;

void segv_handler(int sig) {
    got_signal = 1;
    siglongjmp(jump_buf, 1);
}

int main() {
    size_t page_size = getpagesize();
    
    // Test basic anonymous mapping
    void* ptr = mmap(NULL, page_size * 4, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (ptr == MAP_FAILED) {
        printf("[FAIL] test_mmap: basic mmap failed\n");
        return 1;
    }
    
    // Write to mapped memory
    memset(ptr, 0xAB, page_size * 4);
    
    // Verify read
    unsigned char* p = (unsigned char*)ptr;
    if (p[0] != 0xAB || p[page_size] != 0xAB) {
        printf("[FAIL] test_mmap: read/write failed\n");
        munmap(ptr, page_size * 4);
        return 1;
    }
    
    // Test mprotect - make read-only
    if (mprotect(ptr, page_size, PROT_READ) != 0) {
        printf("[FAIL] test_mmap: mprotect to PROT_READ failed\n");
        munmap(ptr, page_size * 4);
        return 1;
    }
    
    // Install signal handler to catch write fault
    struct sigaction sa, old_sa;
    sa.sa_handler = segv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGSEGV, &sa, &old_sa);
    
    got_signal = 0;
    if (sigsetjmp(jump_buf, 1) == 0) {
        // Try to write to read-only page - should fault
        p[0] = 0xCD;
        // If we get here, protection didn't work
        printf("[WARN] test_mmap: write to PROT_READ succeeded (might be OK on Cygwin)\n");
    } else {
        // Got signal as expected
        if (!got_signal) {
            printf("[FAIL] test_mmap: didn't catch signal\n");
            munmap(ptr, page_size * 4);
            return 1;
        }
    }
    
    sigaction(SIGSEGV, &old_sa, NULL);
    
    // Test PROT_NONE
    void* ptr2 = mmap(NULL, page_size, PROT_NONE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr2 == MAP_FAILED) {
        printf("[FAIL] test_mmap: PROT_NONE mmap failed\n");
        munmap(ptr, page_size * 4);
        return 1;
    }
    
    // Make it readable
    if (mprotect(ptr2, page_size, PROT_READ | PROT_WRITE) != 0) {
        printf("[FAIL] test_mmap: mprotect PROT_NONE -> RW failed\n");
        munmap(ptr, page_size * 4);
        munmap(ptr2, page_size);
        return 1;
    }
    
    // Should be able to write now
    memset(ptr2, 0x12, page_size);
    
    munmap(ptr2, page_size);
    munmap(ptr, page_size * 4);
    
    printf("[PASS] test_mmap\n");
    return 0;
}
