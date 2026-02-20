// Level 1: Cygwin Compatibility - Function Stubs
// Tests that stubbed/unavailable functions don't crash

#define _GNU_SOURCE
#include <sched.h>
#include <cstdio>
#include <cerrno>
#include <unistd.h>

int main() {
    // sched_yield should work
    int ret = sched_yield();
    if (ret != 0 && errno != ENOSYS) {
        printf("[WARN] test_stubs: sched_yield failed: %d\n", errno);
    }
    
    // sched_getscheduler - may return ENOSYS on Cygwin
    ret = sched_getscheduler(0);
    if (ret < 0 && errno != ENOSYS && errno != EPERM) {
        printf("[WARN] test_stubs: sched_getscheduler failed: %d\n", errno);
    }
    
    // CPU affinity - likely not supported on Cygwin
#ifdef CPU_SETSIZE
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    
    // These may return ENOSYS or EINVAL on Cygwin - that's acceptable
    ret = sched_setaffinity(0, sizeof(cpuset), &cpuset);
    if (ret != 0 && errno != ENOSYS && errno != EINVAL && errno != EPERM) {
        printf("[WARN] test_stubs: sched_setaffinity unexpected error: %d\n", errno);
    }
    
    ret = sched_getaffinity(0, sizeof(cpuset), &cpuset);
    if (ret != 0 && errno != ENOSYS && errno != EINVAL && errno != EPERM) {
        printf("[WARN] test_stubs: sched_getaffinity unexpected error: %d\n", errno);
    }
#endif
    
    // getpagesize should work
    int page_size = getpagesize();
    if (page_size < 4096) {
        printf("[FAIL] test_stubs: getpagesize returned %d\n", page_size);
        return 1;
    }
    
    // sysconf for page size
    long sc_page_size = sysconf(_SC_PAGESIZE);
    if (sc_page_size != page_size) {
        printf("[WARN] test_stubs: sysconf(_SC_PAGESIZE) != getpagesize()\n");
    }
    
    // sysconf for number of processors
    long nproc = sysconf(_SC_NPROCESSORS_ONLN);
    if (nproc < 1) {
        printf("[WARN] test_stubs: _SC_NPROCESSORS_ONLN returned %ld\n", nproc);
    }
    
    printf("[PASS] test_stubs\n");
    return 0;
}
