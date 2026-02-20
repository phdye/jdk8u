// Level 6: Memory Management - Arena Allocator
// Tests bump-pointer allocation pattern used by JVM

#define _GNU_SOURCE
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <random>

class Arena {
    std::vector<char*> chunks;
    char* current;
    size_t remaining;
    
public:
    size_t total_allocated;
    size_t total_requested;
    
    Arena() : current(NULL), remaining(0), total_allocated(0), total_requested(0) {}
    
    ~Arena() {
        for (char* c : chunks) free(c);
    }
    
    void* allocate(size_t size) {
        total_requested += size;
        size = (size + 7) & ~7;  // 8-byte align
        
        if (remaining < size) {
            size_t chunk_size = std::max(size_t(64 * 1024), size);
            current = (char*)malloc(chunk_size);
            if (!current) return NULL;
            chunks.push_back(current);
            remaining = chunk_size;
            total_allocated += chunk_size;
        }
        
        void* result = current;
        current += size;
        remaining -= size;
        return result;
    }
    
    void reset() {
        // Reset to beginning of first chunk
        if (!chunks.empty()) {
            current = chunks[0];
            remaining = 64 * 1024;  // First chunk size
        }
    }
};

int main() {
    Arena arena;
    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> size_dist(1, 4096);
    
    // Allocate many random-sized objects
    const int NUM_ALLOCS = 10000;
    std::vector<void*> ptrs;
    
    for (int i = 0; i < NUM_ALLOCS; i++) {
        size_t size = size_dist(rng);
        void* ptr = arena.allocate(size);
        
        if (ptr == NULL) {
            printf("[FAIL] test_arena: allocation %d failed\n", i);
            return 1;
        }
        
        // Touch the memory
        memset(ptr, i & 0xFF, size);
        ptrs.push_back(ptr);
    }
    
    // Verify all pointers are still valid (within chunks)
    // and don't overlap
    for (size_t i = 0; i + 1 < ptrs.size(); i++) {
        if (ptrs[i] == ptrs[i+1]) {
            printf("[FAIL] test_arena: duplicate pointers\n");
            return 1;
        }
    }
    
    // Check overhead
    double overhead = (double)arena.total_allocated / arena.total_requested;
    if (overhead > 2.0) {
        printf("[FAIL] test_arena: overhead %.2fx too high\n", overhead);
        return 1;
    }
    
    printf("[PASS] test_arena (requested=%zuKB, allocated=%zuKB, overhead=%.2fx)\n",
           arena.total_requested / 1024, arena.total_allocated / 1024, overhead);
    return 0;
}
