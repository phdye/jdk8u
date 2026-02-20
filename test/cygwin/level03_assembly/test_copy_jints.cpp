// Level 3: Assembly Stubs - 32-bit Int Copy
// Tests _Copy_arrayof_conjoint_jints from cygwin_x86_64.s

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstddef>

extern "C" {
    void _Copy_arrayof_conjoint_jints(const void* from, void* to, size_t count);
}

int main() {
    // Allocate aligned arrays
    alignas(4) int32_t src[32];
    alignas(4) int32_t dst[32];
    
    // Initialize with distinct values
    for (int i = 0; i < 32; i++) {
        src[i] = i * 12345;
    }
    memset(dst, 0, sizeof(dst));
    
    // Copy all elements
    _Copy_arrayof_conjoint_jints(src, dst, 32);
    
    for (int i = 0; i < 32; i++) {
        if (dst[i] != src[i]) {
            printf("[FAIL] test_copy_jints: mismatch at %d: got %d, expected %d\n",
                   i, dst[i], src[i]);
            return 1;
        }
    }
    
    // Test various counts
    for (int count = 1; count <= 16; count++) {
        memset(dst, 0xFF, sizeof(dst));
        _Copy_arrayof_conjoint_jints(src, dst, count);
        
        for (int i = 0; i < count; i++) {
            if (dst[i] != src[i]) {
                printf("[FAIL] test_copy_jints: count %d mismatch at %d\n", count, i);
                return 1;
            }
        }
        // Check we didn't overwrite
        if ((uint32_t)dst[count] != 0xFFFFFFFFU) {
            printf("[FAIL] test_copy_jints: count %d overwrote past end\n", count);
            return 1;
        }
    }
    
    printf("[PASS] test_copy_jints\n");
    return 0;
}
