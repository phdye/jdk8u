// Level 5: Threading Primitives - Thread-Local Storage
// Tests __thread and pthread_key_t

#define _GNU_SOURCE
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

// C11/GCC style TLS
__thread int tls_var = 0;

// pthread key style TLS
pthread_key_t key;
int destructor_called = 0;
pthread_mutex_t destructor_mutex = PTHREAD_MUTEX_INITIALIZER;

void key_destructor(void* value) {
    pthread_mutex_lock(&destructor_mutex);
    destructor_called++;
    pthread_mutex_unlock(&destructor_mutex);
    free(value);
}

const int NUM_THREADS = 4;

void* tls_thread(void* arg) {
    int id = *(int*)arg;
    
    // Test __thread
    tls_var = id * 100;
    usleep(1000);  // Let other threads run
    
    if (tls_var != id * 100) {
        printf("[FAIL] test_tls: __thread value corrupted in thread %d\n", id);
        _exit(1);
    }
    
    // Test pthread_key
    int* key_val = (int*)malloc(sizeof(int));
    *key_val = id * 200;
    pthread_setspecific(key, key_val);
    
    usleep(1000);
    
    int* retrieved = (int*)pthread_getspecific(key);
    if (retrieved == NULL || *retrieved != id * 200) {
        printf("[FAIL] test_tls: pthread_key value corrupted in thread %d\n", id);
        _exit(1);
    }
    
    return NULL;
}

int main() {
    // Initialize pthread key with destructor
    if (pthread_key_create(&key, key_destructor) != 0) {
        printf("[FAIL] test_tls: pthread_key_create failed\n");
        return 1;
    }
    
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, tls_thread, &ids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // All destructors should have been called
    if (destructor_called != NUM_THREADS) {
        printf("[FAIL] test_tls: destructors called %d times, expected %d\n",
               destructor_called, NUM_THREADS);
        return 1;
    }
    
    pthread_key_delete(key);
    printf("[PASS] test_tls\n");
    return 0;
}
