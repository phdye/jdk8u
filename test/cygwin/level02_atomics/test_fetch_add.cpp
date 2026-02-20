// Level 2: Atomic Operations - Fetch-and-Add Stress Test
// Tests atomic increment/decrement under contention

#include <atomic>
#include <pthread.h>
#include <cstdio>
#include <cstdint>

std::atomic<int64_t> counter(0);
const int THREADS = 8;
const int OPS_PER_THREAD = 100000;

void* add_thread(void* arg) {
    for (int i = 0; i < OPS_PER_THREAD; i++) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
    return NULL;
}

void* sub_thread(void* arg) {
    for (int i = 0; i < OPS_PER_THREAD; i++) {
        counter.fetch_sub(1, std::memory_order_relaxed);
    }
    return NULL;
}

int main() {
    pthread_t threads[THREADS * 2];
    
    // Half add, half subtract - should net to zero
    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, add_thread, NULL);
        pthread_create(&threads[THREADS + i], NULL, sub_thread, NULL);
    }
    
    for (int i = 0; i < THREADS * 2; i++) {
        pthread_join(threads[i], NULL);
    }
    
    int64_t final_val = counter.load();
    if (final_val != 0) {
        printf("[FAIL] test_fetch_add: expected 0, got %lld\n", (long long)final_val);
        return 1;
    }
    
    // Test fetch_add return value
    counter.store(100);
    int64_t old = counter.fetch_add(50);
    if (old != 100 || counter.load() != 150) {
        printf("[FAIL] test_fetch_add: return value incorrect\n");
        return 1;
    }
    
    printf("[PASS] test_fetch_add\n");
    return 0;
}
