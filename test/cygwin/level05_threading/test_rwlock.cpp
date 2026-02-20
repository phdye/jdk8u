// Level 5: Threading Primitives - Read-Write Lock
// Tests pthread_rwlock with concurrent readers and writers

#define _GNU_SOURCE
#include <pthread.h>
#include <cstdio>
#include <atomic>
#include <unistd.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
std::atomic<int> readers(0);
std::atomic<int> writers(0);
int data = 0;
const int NUM_READERS = 4;
const int NUM_WRITERS = 2;
const int OPS = 1000;

void* reader_thread(void* arg) {
    for (int i = 0; i < OPS; i++) {
        pthread_rwlock_rdlock(&rwlock);
        readers++;
        
        // Verify no writers while reading
        if (writers.load() > 0) {
            printf("[FAIL] test_rwlock: writer active during read\n");
            _exit(1);
        }
        
        volatile int v = data;  // Read
        (void)v;
        
        readers--;
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

void* writer_thread(void* arg) {
    for (int i = 0; i < OPS; i++) {
        pthread_rwlock_wrlock(&rwlock);
        writers++;
        
        // Verify exclusive access
        if (readers.load() > 0 || writers.load() > 1) {
            printf("[FAIL] test_rwlock: concurrent access during write\n");
            _exit(1);
        }
        
        data++;  // Write
        
        writers--;
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}

int main() {
    pthread_t rthreads[NUM_READERS], wthreads[NUM_WRITERS];
    
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_create(&rthreads[i], NULL, reader_thread, NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&wthreads[i], NULL, writer_thread, NULL);
    }
    
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(rthreads[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(wthreads[i], NULL);
    }
    
    int expected = NUM_WRITERS * OPS;
    if (data != expected) {
        printf("[FAIL] test_rwlock: data=%d, expected %d\n", data, expected);
        return 1;
    }
    
    printf("[PASS] test_rwlock\n");
    return 0;
}
