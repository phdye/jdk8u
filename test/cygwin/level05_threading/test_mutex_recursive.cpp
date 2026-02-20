// Level 5: Threading Primitives - Recursive Mutex
// Tests PTHREAD_MUTEX_RECURSIVE

#include <pthread.h>
#include <cstdio>
#include <cerrno>

int main() {
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
    
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
    
    // Lock multiple times
    const int LOCK_COUNT = 10;
    for (int i = 0; i < LOCK_COUNT; i++) {
        if (pthread_mutex_lock(&mutex) != 0) {
            printf("[FAIL] test_mutex_recursive: lock %d failed\n", i);
            return 1;
        }
    }
    
    // Trylock should also work
    if (pthread_mutex_trylock(&mutex) != 0) {
        printf("[FAIL] test_mutex_recursive: trylock failed\n");
        return 1;
    }
    
    // Unlock all
    for (int i = 0; i <= LOCK_COUNT; i++) {
        if (pthread_mutex_unlock(&mutex) != 0) {
            printf("[FAIL] test_mutex_recursive: unlock %d failed\n", i);
            return 1;
        }
    }
    
    // Now it should be unlocked - another thread should be able to lock it
    // We test this by trylock succeeding
    if (pthread_mutex_trylock(&mutex) != 0) {
        printf("[FAIL] test_mutex_recursive: final trylock failed\n");
        return 1;
    }
    pthread_mutex_unlock(&mutex);
    
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);
    
    printf("[PASS] test_mutex_recursive\n");
    return 0;
}
