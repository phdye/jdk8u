// Level 2: Atomic Operations - Exchange
// Tests atomic exchange operation

#include <atomic>
#include <pthread.h>
#include <cstdio>
#include <cstdint>

std::atomic<int> shared_val(0);
const int NUM_THREADS = 4;
const int ITERATIONS = 10000;

void* exchange_thread(void* arg) {
    int id = *(int*)arg;
    
    for (int i = 0; i < ITERATIONS; i++) {
        // Exchange our ID into the shared value
        int old = shared_val.exchange(id, std::memory_order_acq_rel);
        (void)old;
    }
    
    return NULL;
}

int main() {
    // Basic exchange test
    std::atomic<int> val(100);
    int old = val.exchange(200);
    if (old != 100) {
        printf("[FAIL] test_exchange: old value wrong: %d\n", old);
        return 1;
    }
    if (val.load() != 200) {
        printf("[FAIL] test_exchange: new value wrong: %d\n", val.load());
        return 1;
    }
    
    // Pointer exchange
    int a = 1, b = 2;
    std::atomic<int*> ptr(&a);
    int* old_ptr = ptr.exchange(&b);
    if (old_ptr != &a) {
        printf("[FAIL] test_exchange: old pointer wrong\n");
        return 1;
    }
    if (ptr.load() != &b) {
        printf("[FAIL] test_exchange: new pointer wrong\n");
        return 1;
    }
    
    // Multi-threaded exchange stress test
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, exchange_thread, &ids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Final value should be one of the thread IDs (1-4)
    int final_val = shared_val.load();
    if (final_val < 1 || final_val > NUM_THREADS) {
        printf("[FAIL] test_exchange: unexpected final value: %d\n", final_val);
        return 1;
    }
    
    printf("[PASS] test_exchange\n");
    return 0;
}
