// Level 1: Cygwin Compatibility - File I/O
// Tests POSIX file I/O functions and flags

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include <cerrno>

int main() {
    const char* testfile = "/tmp/level1_fileio_test.tmp";
    
    // Test O_* flags exist
    int flags = O_RDWR | O_CREAT | O_TRUNC;
#ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#endif
    
    // Test open
    int fd = open(testfile, flags, 0644);
    if (fd < 0) {
        printf("[FAIL] test_file_io: open failed: %s\n", strerror(errno));
        return 1;
    }
    
    // Test write
    const char* data = "Hello, Cygwin file I/O!";
    ssize_t written = write(fd, data, strlen(data));
    if (written != (ssize_t)strlen(data)) {
        printf("[FAIL] test_file_io: write failed\n");
        close(fd);
        unlink(testfile);
        return 1;
    }
    
    // Test lseek with 64-bit offset
    off_t pos = lseek(fd, 0, SEEK_SET);
    if (pos != 0) {
        printf("[FAIL] test_file_io: lseek failed\n");
        close(fd);
        unlink(testfile);
        return 1;
    }
    
    // Test read
    char buf[64] = {0};
    ssize_t readn = read(fd, buf, sizeof(buf) - 1);
    if (readn != written || strcmp(buf, data) != 0) {
        printf("[FAIL] test_file_io: read mismatch\n");
        close(fd);
        unlink(testfile);
        return 1;
    }
    
    // Test fstat
    struct stat st;
    if (fstat(fd, &st) != 0) {
        printf("[FAIL] test_file_io: fstat failed\n");
        close(fd);
        unlink(testfile);
        return 1;
    }
    
    if (st.st_size != written) {
        printf("[FAIL] test_file_io: fstat size mismatch\n");
        close(fd);
        unlink(testfile);
        return 1;
    }
    
    // Test fsync
    if (fsync(fd) != 0) {
        printf("[WARN] test_file_io: fsync failed (may be OK on some filesystems)\n");
    }
    
    close(fd);
    
    // Test stat (not fstat)
    if (stat(testfile, &st) != 0) {
        printf("[FAIL] test_file_io: stat failed\n");
        unlink(testfile);
        return 1;
    }
    
    // Test unlink
    if (unlink(testfile) != 0) {
        printf("[FAIL] test_file_io: unlink failed\n");
        return 1;
    }
    
    printf("[PASS] test_file_io\n");
    return 0;
}
