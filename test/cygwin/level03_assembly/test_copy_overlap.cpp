// Level 3: Assembly Stubs - Overlapping Copy
// Tests conjoint (overlapping) copy behavior

#include <cstdio>
#include <cstring>
#include <cstddef>

extern "C" {
    void _Copy_arrayof_conjoint_bytes(const void* from, void* to, size_t count);
}

int main() {
    char buf[64];
    char ref[64];
    
    // Test forward overlap (src < dst, overlap in middle)
    // Source: ABCDEFGHIJKLMNOP
    // Copy buf[0..11] to buf[4..15]
    // Expected: ABCDABCDEFGHIJKL (memmove behavior)
    strcpy(buf, "ABCDEFGHIJKLMNOP");
    strcpy(ref, "ABCDEFGHIJKLMNOP");
    
    _Copy_arrayof_conjoint_bytes(buf, buf + 4, 12);
    memmove(ref + 4, ref, 12);  // Reference
    
    if (memcmp(buf, ref, 16) != 0) {
        printf("[FAIL] test_copy_overlap: forward overlap\n");
        printf("  Got: %.16s\n", buf);
        printf("  Exp: %.16s\n", ref);
        return 1;
    }
    
    // Test backward overlap (src > dst, overlap in middle)
    // Source: ABCDEFGHIJKLMNOP
    // Copy buf[4..15] to buf[0..11]
    // Expected: EFGHIJKLMNOPMNOP
    strcpy(buf, "ABCDEFGHIJKLMNOP");
    strcpy(ref, "ABCDEFGHIJKLMNOP");
    
    _Copy_arrayof_conjoint_bytes(buf + 4, buf, 12);
    memmove(ref, ref + 4, 12);  // Reference
    
    if (memcmp(buf, ref, 16) != 0) {
        printf("[FAIL] test_copy_overlap: backward overlap\n");
        printf("  Got: %.16s\n", buf);
        printf("  Exp: %.16s\n", ref);
        return 1;
    }
    
    // Test complete overlap (dst == src) - should be no-op
    strcpy(buf, "ABCDEFGHIJKLMNOP");
    _Copy_arrayof_conjoint_bytes(buf, buf, 16);
    if (strcmp(buf, "ABCDEFGHIJKLMNOP") != 0) {
        printf("[FAIL] test_copy_overlap: self copy\n");
        return 1;
    }
    
    printf("[PASS] test_copy_overlap\n");
    return 0;
}
