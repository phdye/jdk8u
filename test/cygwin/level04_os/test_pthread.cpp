// Level 4: OS Abstraction - pthread
// Tests pthread thread creation with various attributes

#define _GNU_SOURCE
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>

static int thread_ran = 0;

void* thread_func(void* arg) {
    int* val = (int*)arg;
    thread_ran = 1;
    return (void*)(intptr_t)(*val * 2);
}

int main() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    // Set stack size
    size_t stack_size = 512 * 1024;  // 512KB
    if (pthread_attr_setstacksize(&attr, stack_size) != 0) {
        printf("[FAIL] test_pthread: setstacksize failed\n");
        return 1;
    }
    
    size_t got_size;
    pthread_attr_getstacksize(&attr, &got_size);
    if (got_size < stack_size) {
        printf("[FAIL] test_pthread: stack size not set correctly\n");
        return 1;
    }
    
    // Set guard size (must be > 0 on Cygwin)
    size_t guard_size = getpagesize();
    if (pthread_attr_setguardsize(&attr, guard_size) != 0) {
        printf("[FAIL] test_pthread: setguardsize failed\n");
        return 1;
    }
    
    // Set detach state
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    // Create thread
    pthread_t thread;
    int arg = 21;
    thread_ran = 0;
    
    int ret = pthread_create(&thread, &attr, thread_func, &arg);
    if (ret != 0) {
        printf("[FAIL] test_pthread: pthread_create failed: %d\n", ret);
        return 1;
    }
    
    // Join and check result
    void* result;
    ret = pthread_join(thread, &result);
    if (ret != 0) {
        printf("[FAIL] test_pthread: pthread_join failed: %d\n", ret);
        return 1;
    }
    
    if (!thread_ran) {
        printf("[FAIL] test_pthread: thread did not run\n");
        return 1;
    }
    
    if ((intptr_t)result != 42) {
        printf("[FAIL] test_pthread: wrong return value: %ld\n", (long)(intptr_t)result);
        return 1;
    }
    
    pthread_attr_destroy(&attr);
    
    printf("[PASS] test_pthread\n");
    return 0;
}
