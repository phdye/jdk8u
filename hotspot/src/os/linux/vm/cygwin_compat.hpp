/*
 * Cygwin compatibility header for OpenJDK HotSpot
 *
 * Cygwin provides 64-bit file I/O by default (off_t is 64-bit),
 * so the Linux-specific *64 variants don't exist. This header
 * maps them to their standard POSIX equivalents.
 *
 * It also provides missing POSIX/Linux constants that Cygwin
 * does not define.
 */
#ifndef CYGWIN_COMPAT_HPP
#define CYGWIN_COMPAT_HPP

#ifdef __CYGWIN__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* ===== 64-bit file I/O mappings ===== */
/* Simple token replacement so both "struct stat64" and "::stat64()"
   are mapped to their non-64 equivalents. */

typedef off_t off64_t;

#define stat64    stat
#define fstat64   fstat
#define open64    open
#define fopen64   fopen
#define lseek64   lseek
#define ftruncate64 ftruncate
#define mmap64    mmap

/* ===== Missing resource limit constants ===== */

#ifndef RLIMIT_NPROC
#define RLIMIT_NPROC 6
#endif

/* ===== Missing signal constants ===== */

#ifndef SA_NOCLDWAIT
#define SA_NOCLDWAIT 0
#endif

#ifndef TRAP_BRKPT
#define TRAP_BRKPT 1
#endif
#ifndef TRAP_TRACE
#define TRAP_TRACE 2
#endif

#ifndef POLL_OUT
#define POLL_OUT 2
#endif
#ifndef POLL_MSG
#define POLL_MSG 3
#endif
#ifndef POLL_ERR
#define POLL_ERR 4
#endif
#ifndef POLL_PRI
#define POLL_PRI 5
#endif
#ifndef POLL_HUP
#define POLL_HUP 6
#endif

/* ===== Missing network constants ===== */

#ifndef AF_PACKET
#define AF_PACKET 17
#endif

/* ===== Missing shared memory constants ===== */
/*
 * These are Linux-specific SHM flags not available on Cygwin.
 * Huge pages (SHM_HUGETLB) are not supported on Cygwin.
 * Defining these allows the code to compile; shmget() will fail
 * at runtime which is the correct behavior.
 */

#ifndef SHM_HUGETLB
#define SHM_HUGETLB 04000  /* Linux huge page flag */
#endif

#ifndef SHM_R
#define SHM_R 0400  /* Owner read permission */
#endif

#ifndef SHM_W
#define SHM_W 0200  /* Owner write permission */
#endif

#ifndef SHM_REMAP
#define SHM_REMAP 040000  /* Linux remap flag */
#endif

/* ===== syscall.h alternatives ===== */
/*
 * Cygwin doesn't have syscall.h because it's a userspace POSIX layer,
 * not a Linux kernel. Raw syscalls have no meaning - there's no Linux
 * kernel to receive them. See issue/syscall-h-research.md for details.
 *
 * We provide POSIX-based alternatives for the syscalls used by HotSpot.
 */

#include <pthread.h>
#include <windows.h>

/* Undefine problematic macros from windows.h that conflict with HotSpot code */
#ifdef MAX_PATH
#undef MAX_PATH
#endif

/* COM headers define 'interface' as 'struct', breaks C++ code using 'interface' as variable */
#ifdef interface
#undef interface
#endif

/* Other common Windows macro conflicts */
#ifdef small
#undef small
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

/* Windows print dialog defines INTERFACE macro, conflicts with port.hpp */
#ifdef INTERFACE
#undef INTERFACE
#endif

/*
 * gettid() alternative - returns a thread identifier.
 *
 * On Linux, gettid() returns a kernel-assigned pid_t unique across the system.
 * On Cygwin, we use GetCurrentThreadId() which returns a Windows thread ID.
 * This is suitable for logging/debugging but NOT identical to Linux semantics.
 */
static inline pid_t cygwin_gettid(void) {
    return (pid_t)GetCurrentThreadId();
}

/*
 * sys_clock_getres via syscall is not available.
 * Return -1 to disable fast_thread_cpu_time (safe fallback).
 */
#define sys_clock_getres(x, y) (-1)

/*
 * sched_getcpu via syscall is not available.
 * The fallback in os_linux.cpp will handle this gracefully.
 */

/* ===== gnu/libc-version.h stub ===== */
/*
 * Cygwin uses newlib, not glibc, so these functions don't exist.
 * Provide stubs that return safe values.
 */
static inline const char* gnu_get_libc_version(void) {
    return "cygwin";  /* Not glibc */
}

static inline const char* gnu_get_libc_release(void) {
    return "stable";  /* Cygwin doesn't have glibc release info */
}

static inline const char* gnu_get_libpthread_version(void) {
    return "NPTL";  /* Cygwin pthreads are NPTL-like */
}

/* ===== link.h stub ===== */
/*
 * Cygwin uses PE/COFF format, not ELF. The link.h header provides
 * ELF dynamic linker structures for iterating loaded shared libraries.
 * We stub dl_iterate_phdr to return 0 (no libraries found).
 */

struct dl_phdr_info {
    void*       dlpi_addr;
    const char* dlpi_name;
    void*       dlpi_phdr;
    short       dlpi_phnum;
};

typedef int (*dl_iterate_phdr_callback)(struct dl_phdr_info*, size_t, void*);

static inline int dl_iterate_phdr(dl_iterate_phdr_callback callback, void* data) {
    (void)callback;
    (void)data;
    return 0;  /* No shared libraries found (stub) */
}

#endif /* __CYGWIN__ */

#endif /* CYGWIN_COMPAT_HPP */
