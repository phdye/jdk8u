// Level 4: OS Abstraction - Pipes and File Descriptors
// Tests pipe(), read(), write(), select()

#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <sys/select.h>
#include <cerrno>

int main() {
    int pipefd[2];
    
    // Create pipe
    if (pipe(pipefd) != 0) {
        printf("[FAIL] test_pipes: pipe() failed\n");
        return 1;
    }
    
    // Set read end to non-blocking
    int flags = fcntl(pipefd[0], F_GETFL);
    if (flags == -1) {
        printf("[FAIL] test_pipes: F_GETFL failed\n");
        return 1;
    }
    
    if (fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK) == -1) {
        printf("[FAIL] test_pipes: F_SETFL failed\n");
        return 1;
    }
    
    // Write to pipe
    const char* msg = "Hello, pipe!";
    ssize_t written = write(pipefd[1], msg, strlen(msg));
    if (written != (ssize_t)strlen(msg)) {
        printf("[FAIL] test_pipes: write failed\n");
        return 1;
    }
    
    // Read from pipe
    char buf[64] = {0};
    ssize_t readn = read(pipefd[0], buf, sizeof(buf) - 1);
    if (readn != written) {
        printf("[FAIL] test_pipes: read wrong size: %zd vs %zd\n", readn, written);
        return 1;
    }
    
    if (strcmp(buf, msg) != 0) {
        printf("[FAIL] test_pipes: read mismatch\n");
        return 1;
    }
    
    // Non-blocking read on empty pipe should return EAGAIN
    readn = read(pipefd[0], buf, sizeof(buf));
    if (readn != -1 || (errno != EAGAIN && errno != EWOULDBLOCK)) {
        printf("[FAIL] test_pipes: empty read should fail with EAGAIN\n");
        return 1;
    }
    
    // Test select() on pipe
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(pipefd[0], &read_fds);
    
    struct timeval tv = {0, 0};  // Immediate return
    int ready = select(pipefd[0] + 1, &read_fds, NULL, NULL, &tv);
    
    if (ready != 0) {
        printf("[FAIL] test_pipes: select on empty pipe returned %d\n", ready);
        return 1;
    }
    
    // Write something and check select again
    write(pipefd[1], "x", 1);
    
    FD_ZERO(&read_fds);
    FD_SET(pipefd[0], &read_fds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    ready = select(pipefd[0] + 1, &read_fds, NULL, NULL, &tv);
    if (ready != 1 || !FD_ISSET(pipefd[0], &read_fds)) {
        printf("[FAIL] test_pipes: select after write returned %d\n", ready);
        return 1;
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
    
    printf("[PASS] test_pipes\n");
    return 0;
}
