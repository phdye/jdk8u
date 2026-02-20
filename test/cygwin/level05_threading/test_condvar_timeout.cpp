// Level 5: Threading Primitives - Condition Variable Timeout
// Tests pthread_cond_timedwait

#define _GNU_SOURCE
#include <pthread.h>
#include <cstdio>
#include <ctime>
#include <cerrno>

int main() {
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    
    pthread_mutex_lock(&mutex);
    
    // Get current time and add 50ms
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += 50000000;  // 50ms
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
    }
    
    struct timespec before, after;
    clock_gettime(CLOCK_MONOTONIC, &before);
    
    int ret = pthread_cond_timedwait(&cond, &mutex, &ts);
    
    clock_gettime(CLOCK_MONOTONIC, &after);
    
    pthread_mutex_unlock(&mutex);
    
    // Should timeout
    if (ret != ETIMEDOUT) {
        printf("[FAIL] test_condvar_timeout: expected ETIMEDOUT, got %d\n", ret);
        return 1;
    }
    
    // Check elapsed time
    long elapsed_ms = (after.tv_sec - before.tv_sec) * 1000 +
                      (after.tv_nsec - before.tv_nsec) / 1000000;
    
    // Should be close to 50ms (allow 30-200ms range for scheduler variance)
    if (elapsed_ms < 30 || elapsed_ms > 200) {
        printf("[FAIL] test_condvar_timeout: elapsed %ldms, expected ~50ms\n", elapsed_ms);
        return 1;
    }
    
    printf("[PASS] test_condvar_timeout (elapsed=%ldms)\n", elapsed_ms);
    return 0;
}
