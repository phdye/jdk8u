// Level 3: Assembly Stubs - Byte Copy
// Tests _Copy_arrayof_conjoint_bytes from cygwin_x86_64.s

#include <cstdio>
#include <cstring>
#include <cstddef>

extern "C" {
    void _Copy_arrayof_conjoint_bytes(const void* from, void* to, size_t count);
}

int main() {
    // Basic non-overlapping copy
    char src[64];
    char dst[64];
    
    for (int i = 0; i < 64; i++) src[i] = (char)i;
    memset(dst, 0, 64);
    
    _Copy_arrayof_conjoint_bytes(src, dst, 64);
    
    for (int i = 0; i < 64; i++) {
        if (dst[i] != (char)i) {
            printf("[FAIL] test_copy_bytes: basic copy mismatch at %d\n", i);
            return 1;
        }
    }
    
    // Test various sizes (1-63 bytes)
    for (int size = 1; size < 64; size++) {
        memset(dst, 0xFF, 64);
        _Copy_arrayof_conjoint_bytes(src, dst, size);
        
        for (int i = 0; i < size; i++) {
            if (dst[i] != (char)i) {
                printf("[FAIL] test_copy_bytes: size %d mismatch at %d\n", size, i);
                return 1;
            }
        }
        // Verify we didn't write past the end
        if (dst[size] != (char)0xFF) {
            printf("[FAIL] test_copy_bytes: overwrote past size %d\n", size);
            return 1;
        }
    }
    
    // Zero-length copy should not crash
    _Copy_arrayof_conjoint_bytes(src, dst, 0);
    
    printf("[PASS] test_copy_bytes\n");
    return 0;
}
