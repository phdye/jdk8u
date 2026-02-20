// Level 6: Memory Management - CodeCache Simulation
// Tests executable memory allocation for JIT

#define _GNU_SOURCE
#include <sys/mman.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>

class CodeCache {
    void* base;
    size_t size;
    size_t used;
    
public:
    CodeCache(size_t sz) : used(0) {
        size_t page_size = getpagesize();
        size = (sz + page_size - 1) & ~(page_size - 1);
        
        // Allocate with PROT_EXEC
        base = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    
    ~CodeCache() {
        if (base != MAP_FAILED) munmap(base, size);
    }
    
    bool is_valid() const { return base != MAP_FAILED; }
    
    void* allocate(size_t len) {
        len = (len + 15) & ~15;  // 16-byte align for code
        if (used + len > size) return NULL;
        
        void* result = (char*)base + used;
        used += len;
        return result;
    }
    
    size_t get_used() const { return used; }
    size_t get_size() const { return size; }
    
    bool contains(void* addr) const {
        return addr >= base && addr < (char*)base + size;
    }
};

int main() {
    CodeCache cc(1024 * 1024);  // 1MB
    
    if (!cc.is_valid()) {
        printf("[FAIL] test_codecache: allocation failed\n");
        return 1;
    }
    
    // Allocate some "code" entries
    const int NUM_ENTRIES = 100;
    void* entries[NUM_ENTRIES];
    
    for (int i = 0; i < NUM_ENTRIES; i++) {
        entries[i] = cc.allocate(1024);  // 1KB per method
        if (!entries[i]) {
            printf("[FAIL] test_codecache: entry %d allocation failed\n", i);
            return 1;
        }
        
        // Write some NOPs (0x90 on x86)
        memset(entries[i], 0x90, 1024);
    }
    
    // Verify all entries are in the cache
    for (int i = 0; i < NUM_ENTRIES; i++) {
        if (!cc.contains(entries[i])) {
            printf("[FAIL] test_codecache: entry %d not in cache\n", i);
            return 1;
        }
    }
    
    // Verify entries don't overlap
    for (int i = 0; i < NUM_ENTRIES - 1; i++) {
        char* end_i = (char*)entries[i] + 1024;
        if (end_i > entries[i + 1]) {
            printf("[FAIL] test_codecache: entries %d and %d overlap\n", i, i + 1);
            return 1;
        }
    }
    
    printf("[PASS] test_codecache (used=%zuKB)\n", cc.get_used() / 1024);
    return 0;
}
