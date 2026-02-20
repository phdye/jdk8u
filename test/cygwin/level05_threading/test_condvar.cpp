// Level 5: Threading Primitives - Condition Variable
// Tests pthread_cond_wait, pthread_cond_signal, pthread_cond_broadcast

#define _GNU_SOURCE
#include <pthread.h>
#include <cstdio>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;
int waiting_count = 0;

void* waiter_thread(void* arg) {
    pthread_mutex_lock(&mutex);
    waiting_count++;
    while (!ready) {
        pthread_cond_wait(&cond, &mutex);
    }
    waiting_count--;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t waiter;
    
    // Test signal
    ready = 0;
    waiting_count = 0;
    
    pthread_create(&waiter, NULL, waiter_thread, NULL);
    
    // Wait for thread to start waiting
    while (1) {
        pthread_mutex_lock(&mutex);
        int count = waiting_count;
        pthread_mutex_unlock(&mutex);
        if (count > 0) break;
        usleep(1000);
    }
    
    // Signal the waiter
    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    
    pthread_join(waiter, NULL);
    
    // Test broadcast with multiple waiters
    const int NUM_WAITERS = 4;
    pthread_t waiters[NUM_WAITERS];
    ready = 0;
    waiting_count = 0;
    
    for (int i = 0; i < NUM_WAITERS; i++) {
        pthread_create(&waiters[i], NULL, waiter_thread, NULL);
    }
    
    // Wait for all threads to start waiting
    while (1) {
        pthread_mutex_lock(&mutex);
        int count = waiting_count;
        pthread_mutex_unlock(&mutex);
        if (count >= NUM_WAITERS) break;
        usleep(1000);
    }
    
    // Broadcast to wake all
    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    
    for (int i = 0; i < NUM_WAITERS; i++) {
        pthread_join(waiters[i], NULL);
    }
    
    printf("[PASS] test_condvar\n");
    return 0;
}
