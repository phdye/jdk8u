// Level 1: Cygwin Compatibility - Process/Thread IDs
// Tests getpid, getppid, getuid, pthread_self, etc.

#include <unistd.h>
#include <pthread.h>
#include <cstdio>
#include <sys/types.h>

#ifdef __CYGWIN__
#include "cygwin_compat.hpp"
#endif

int main() {
    // getpid/getppid
    pid_t pid = getpid();
    pid_t ppid = getppid();
    
    if (pid <= 0) {
        printf("[FAIL] test_ids: getpid() returned %d\n", pid);
        return 1;
    }
    
    if (ppid <= 0) {
        printf("[FAIL] test_ids: getppid() returned %d\n", ppid);
        return 1;
    }
    
    // pid and ppid should be different (unless we're init, which we're not)
    if (pid == ppid && pid != 1) {
        printf("[WARN] test_ids: pid == ppid == %d\n", pid);
    }
    
    // getuid/geteuid
    uid_t uid = getuid();
    uid_t euid = geteuid();
    // These can be 0 (root) or positive, both are valid
    (void)uid;
    (void)euid;
    
    // getgid/getegid
    gid_t gid = getgid();
    gid_t egid = getegid();
    (void)gid;
    (void)egid;
    
    // pthread_self - must return non-zero
    pthread_t self = pthread_self();
    if (self == 0) {
        printf("[FAIL] test_ids: pthread_self() returned 0\n");
        return 1;
    }
    
    // pthread_equal should work
    if (!pthread_equal(self, self)) {
        printf("[FAIL] test_ids: pthread_equal(self, self) returned false\n");
        return 1;
    }
    
    printf("[PASS] test_ids\n");
    return 0;
}
