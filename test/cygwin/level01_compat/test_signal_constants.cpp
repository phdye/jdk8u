// Level 1: Cygwin Compatibility - Signal Constants
// Tests that all required signal constants are defined

#define _GNU_SOURCE
#include <signal.h>
#include <cstdio>

int main() {
    // Basic signals
    int signals[] = {
        SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT,
        SIGBUS, SIGFPE, SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2,
        SIGPIPE, SIGALRM, SIGTERM, SIGCHLD, SIGCONT, SIGSTOP,
        SIGTSTP, SIGTTIN, SIGTTOU
    };
    (void)signals;
    
    // Signal action flags
    int sa_flags = SA_SIGINFO | SA_RESTART;
#ifdef SA_NODEFER
    sa_flags |= SA_NODEFER;
#endif
#ifdef SA_RESETHAND
    sa_flags |= SA_RESETHAND;
#endif
#ifdef SA_ONSTACK
    sa_flags |= SA_ONSTACK;
#endif
    (void)sa_flags;
    
    // Trap types (needed for crash handling)
#ifdef TRAP_BRKPT
    int trap = TRAP_BRKPT;
    if (trap <= 0) {
        printf("[FAIL] test_signal_constants: TRAP_BRKPT invalid\n");
        return 1;
    }
#else
    printf("[WARN] test_signal_constants: TRAP_BRKPT not defined\n");
#endif

#ifdef TRAP_TRACE
    int trace = TRAP_TRACE;
    (void)trace;
#endif
    
    // Signal info codes
#ifdef SI_USER
    int si_user = SI_USER;
    (void)si_user;
#endif

#ifdef SI_QUEUE
    int si_queue = SI_QUEUE;
    (void)si_queue;
#endif
    
    printf("[PASS] test_signal_constants\n");
    return 0;
}
