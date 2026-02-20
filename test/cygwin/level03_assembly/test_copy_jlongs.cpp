// Level 3: Assembly Stubs - 64-bit Long Copy
// Tests _Copy_conjoint_jlongs_atomic from cygwin_x86_64.s

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstddef>

extern "C" {
    void _Copy_conjoint_jlongs_atomic(const void* from, void* to, size_t count);
}

int main() {
    // Allocate aligned arrays
    alignas(8) int64_t src[16];
    alignas(8) int64_t dst[16];
    
    // Initialize with distinct values
    for (int i = 0; i < 16; i++) {
        src[i] = (int64_t)i * 1000000000LL + i;
    }
    memset(dst, 0, sizeof(dst));
    
    // Copy all elements
    _Copy_conjoint_jlongs_atomic(src, dst, 16);
    
    for (int i = 0; i < 16; i++) {
        if (dst[i] != src[i]) {
            printf("[FAIL] test_copy_jlongs: mismatch at %d: got %lld, expected %lld\n",
                   i, (long long)dst[i], (long long)src[i]);
            return 1;
        }
    }
    
    // Test partial copy
    memset(dst, 0, sizeof(dst));
    _Copy_conjoint_jlongs_atomic(src, dst, 5);
    
    for (int i = 0; i < 5; i++) {
        if (dst[i] != src[i]) {
            printf("[FAIL] test_copy_jlongs: partial copy mismatch at %d\n", i);
            return 1;
        }
    }
    // Rest should be zero
    for (int i = 5; i < 16; i++) {
        if (dst[i] != 0) {
            printf("[FAIL] test_copy_jlongs: overwrote past count\n");
            return 1;
        }
    }
    
    // Test single element
    dst[0] = 0;
    _Copy_conjoint_jlongs_atomic(src, dst, 1);
    if (dst[0] != src[0]) {
        printf("[FAIL] test_copy_jlongs: single element\n");
        return 1;
    }
    
    // Test zero count
    dst[0] = 999;
    _Copy_conjoint_jlongs_atomic(src, dst, 0);
    if (dst[0] != 999) {
        printf("[FAIL] test_copy_jlongs: zero count modified dst\n");
        return 1;
    }
    
    printf("[PASS] test_copy_jlongs\n");
    return 0;
}
