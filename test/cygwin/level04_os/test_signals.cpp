// Level 4: OS Abstraction - Signal Handling
// Tests sigaction, signal delivery, and signal masks

#define _GNU_SOURCE
#include <signal.h>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <setjmp.h>

static sigjmp_buf jump_buf;
static volatile sig_atomic_t signal_count = 0;
static volatile int last_signal = 0;

void sig_handler(int sig) {
    signal_count++;
    last_signal = sig;
}

void sig_handler_jump(int sig) {
    last_signal = sig;
    siglongjmp(jump_buf, 1);
}

int main() {
    struct sigaction sa;
    
    // Test basic signal delivery
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGUSR1, &sa, NULL) != 0) {
        printf("[FAIL] test_signals: sigaction failed\n");
        return 1;
    }
    
    signal_count = 0;
    last_signal = 0;
    raise(SIGUSR1);
    
    // Give signal time to be delivered
    usleep(1000);
    
    if (signal_count != 1 || last_signal != SIGUSR1) {
        printf("[FAIL] test_signals: signal not delivered (count=%d, sig=%d)\n",
               (int)signal_count, last_signal);
        return 1;
    }
    
    // Test signal masking
    sigset_t block_set, old_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGUSR2);
    
    if (pthread_sigmask(SIG_BLOCK, &block_set, &old_set) != 0) {
        printf("[FAIL] test_signals: pthread_sigmask failed\n");
        return 1;
    }
    
    sa.sa_handler = sig_handler;
    sigaction(SIGUSR2, &sa, NULL);
    
    signal_count = 0;
    raise(SIGUSR2);
    usleep(1000);
    
    // Signal should be pending, not delivered
    if (signal_count != 0) {
        printf("[FAIL] test_signals: blocked signal was delivered\n");
        return 1;
    }
    
    // Check it's pending
    sigset_t pending;
    sigpending(&pending);
    if (!sigismember(&pending, SIGUSR2)) {
        printf("[FAIL] test_signals: signal not pending\n");
        return 1;
    }
    
    // Unblock - signal should be delivered
    pthread_sigmask(SIG_UNBLOCK, &block_set, NULL);
    usleep(1000);
    
    if (signal_count != 1 || last_signal != SIGUSR2) {
        printf("[FAIL] test_signals: unblocked signal not delivered\n");
        return 1;
    }
    
    // Test siglongjmp from signal handler
    sa.sa_handler = sig_handler_jump;
    sigaction(SIGUSR1, &sa, NULL);
    
    if (sigsetjmp(jump_buf, 1) == 0) {
        raise(SIGUSR1);
        printf("[FAIL] test_signals: siglongjmp didn't jump\n");
        return 1;
    } else {
        if (last_signal != SIGUSR1) {
            printf("[FAIL] test_signals: wrong signal after jump\n");
            return 1;
        }
    }
    
    printf("[PASS] test_signals\n");
    return 0;
}
