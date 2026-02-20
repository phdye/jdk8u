// Level 6: Memory Management - VirtualSpace Simulation
// Tests reserve/commit/uncommit pattern used by JVM

#define _GNU_SOURCE
#include <sys/mman.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>

class VirtualSpace {
    void* base;
    size_t reserved_size;
    size_t committed_size;
    size_t page_size;

public:
    VirtualSpace(size_t size) : committed_size(0) {
        page_size = getpagesize();
        reserved_size = (size + page_size - 1) & ~(page_size - 1);
        
        // Reserve with PROT_NONE
        base = mmap(NULL, reserved_size, PROT_NONE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (base == MAP_FAILED) base = NULL;
    }
    
    ~VirtualSpace() {
        if (base) munmap(base, reserved_size);
    }
    
    bool is_valid() const { return base != NULL; }
    void* get_base() const { return base; }
    size_t get_committed() const { return committed_size; }
    size_t get_reserved() const { return reserved_size; }
    
    bool expand(size_t new_size) {
        if (new_size > reserved_size) return false;
        new_size = (new_size + page_size - 1) & ~(page_size - 1);
        
        if (new_size <= committed_size) return true;
        
        char* commit_start = (char*)base + committed_size;
        size_t commit_len = new_size - committed_size;
        
        if (mprotect(commit_start, commit_len, PROT_READ | PROT_WRITE) != 0) {
            return false;
        }
        
        committed_size = new_size;
        return true;
    }
    
    bool shrink(size_t new_size) {
        if (new_size >= committed_size) return true;
        new_size = (new_size + page_size - 1) & ~(page_size - 1);
        
        char* uncommit_start = (char*)base + new_size;
        size_t uncommit_len = committed_size - new_size;
        
        if (mprotect(uncommit_start, uncommit_len, PROT_NONE) != 0) {
            return false;
        }
        
        committed_size = new_size;
        return true;
    }
};

int main() {
    size_t MB = 1024 * 1024;
    VirtualSpace vs(64 * MB);
    
    if (!vs.is_valid()) {
        printf("[FAIL] test_virtualspace: reserve failed\n");
        return 1;
    }
    
    // Expand incrementally
    for (size_t size = MB; size <= 16 * MB; size += MB) {
        if (!vs.expand(size)) {
            printf("[FAIL] test_virtualspace: expand to %zuMB failed\n", size / MB);
            return 1;
        }
        
        // Touch committed memory
        char* ptr = (char*)vs.get_base() + size - 1;
        *ptr = 'X';
    }
    
    // Shrink
    if (!vs.shrink(8 * MB)) {
        printf("[FAIL] test_virtualspace: shrink failed\n");
        return 1;
    }
    
    if (vs.get_committed() != 8 * MB) {
        printf("[FAIL] test_virtualspace: wrong committed size after shrink\n");
        return 1;
    }
    
    // Expand again after shrink
    if (!vs.expand(12 * MB)) {
        printf("[FAIL] test_virtualspace: re-expand failed\n");
        return 1;
    }
    
    printf("[PASS] test_virtualspace\n");
    return 0;
}
