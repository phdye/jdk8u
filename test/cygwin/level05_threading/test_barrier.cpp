// Level 5: Threading Primitives - Barrier
// Tests pthread_barrier synchronization

#define _GNU_SOURCE
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <atomic>

const int NUM_THREADS = 4;
pthread_barrier_t barrier;
std::atomic<int> phase1_count(0);
std::atomic<int> phase2_count(0);

void* barrier_thread(void* arg) {
    // Phase 1
    phase1_count++;
    
    // Wait for all threads
    int ret = pthread_barrier_wait(&barrier);
    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("[FAIL] test_barrier: barrier_wait failed: %d\n", ret);
        _exit(1);
    }
    
    // All threads should have completed phase 1
    if (phase1_count.load() != NUM_THREADS) {
        printf("[FAIL] test_barrier: not all threads reached phase 1\n");
        _exit(1);
    }
    
    // Phase 2
    phase2_count++;
    
    // Wait again
    ret = pthread_barrier_wait(&barrier);
    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("[FAIL] test_barrier: second barrier_wait failed: %d\n", ret);
        _exit(1);
    }
    
    if (phase2_count.load() != NUM_THREADS) {
        printf("[FAIL] test_barrier: not all threads reached phase 2\n");
        _exit(1);
    }
    
    return NULL;
}

int main() {
    if (pthread_barrier_init(&barrier, NULL, NUM_THREADS) != 0) {
        printf("[FAIL] test_barrier: barrier_init failed\n");
        return 1;
    }
    
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, barrier_thread, NULL);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barrier);
    printf("[PASS] test_barrier\n");
    return 0;
}
