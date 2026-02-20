// Level 5: Threading Primitives - Mutex
// Tests pthread_mutex with multi-threaded contention

#include <pthread.h>
#include <cstdio>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_counter = 0;
const int THREADS = 8;
const int INCREMENTS = 10000;

void* mutex_thread(void* arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&mutex);
        shared_counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    
    for (int i = 0; i < THREADS; i++) {
        if (pthread_create(&threads[i], NULL, mutex_thread, NULL) != 0) {
            printf("[FAIL] test_mutex: pthread_create failed\n");
            return 1;
        }
    }
    
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    int expected = THREADS * INCREMENTS;
    if (shared_counter != expected) {
        printf("[FAIL] test_mutex: expected %d, got %d\n", expected, shared_counter);
        return 1;
    }
    
    // Test trylock
    if (pthread_mutex_trylock(&mutex) != 0) {
        printf("[FAIL] test_mutex: trylock on unlocked mutex failed\n");
        return 1;
    }
    
    // Second trylock should fail
    int ret = pthread_mutex_trylock(&mutex);
    if (ret == 0) {
        printf("[FAIL] test_mutex: trylock should have failed\n");
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&mutex);
        return 1;
    }
    
    pthread_mutex_unlock(&mutex);
    
    printf("[PASS] test_mutex\n");
    return 0;
}
