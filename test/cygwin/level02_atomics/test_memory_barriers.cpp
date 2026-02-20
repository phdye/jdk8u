// Level 2: Atomic Operations - Memory Barriers
// Tests store-load barrier ordering (the hardest to get right)

#include <atomic>
#include <pthread.h>
#include <cstdio>

// Dekker's algorithm style test for store-load ordering
std::atomic<int> x(0), y(0);
std::atomic<int> r1(0), r2(0);

void* thread1(void* arg) {
    x.store(1, std::memory_order_seq_cst);
    r1.store(y.load(std::memory_order_seq_cst), std::memory_order_relaxed);
    return NULL;
}

void* thread2(void* arg) {
    y.store(1, std::memory_order_seq_cst);
    r2.store(x.load(std::memory_order_seq_cst), std::memory_order_relaxed);
    return NULL;
}

int main() {
    // Run many iterations to catch ordering bugs
    int both_zero = 0;
    const int ITERATIONS = 10000;
    
    for (int i = 0; i < ITERATIONS; i++) {
        x.store(0, std::memory_order_relaxed);
        y.store(0, std::memory_order_relaxed);
        r1.store(-1, std::memory_order_relaxed);
        r2.store(-1, std::memory_order_relaxed);
        
        std::atomic_thread_fence(std::memory_order_seq_cst);
        
        pthread_t t1, t2;
        pthread_create(&t1, NULL, thread1, NULL);
        pthread_create(&t2, NULL, thread2, NULL);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        
        int v1 = r1.load(std::memory_order_relaxed);
        int v2 = r2.load(std::memory_order_relaxed);
        
        // With seq_cst, we should never see r1==0 && r2==0
        // That would mean both loads happened before both stores
        if (v1 == 0 && v2 == 0) {
            both_zero++;
        }
    }
    
    if (both_zero > 0) {
        printf("[FAIL] test_memory_barriers: %d ordering violations in %d iterations\n",
               both_zero, ITERATIONS);
        return 1;
    }
    
    printf("[PASS] test_memory_barriers\n");
    return 0;
}
