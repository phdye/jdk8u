/*
 * Cygwin compatibility definitions for JDK native code.
 * Cygwin has 64-bit off_t by default, so *64 functions are not needed.
 */

#ifndef CYGWIN_COMPAT_H
#define CYGWIN_COMPAT_H

#ifdef __CYGWIN__

#include <sys/ioctl.h>  /* for ioctl, FIONREAD */
#include <dirent.h>
#include <sys/statvfs.h>

/* 64-bit file operations - Cygwin uses standard functions */
#define dirent64 dirent
#define readdir64 readdir
#define readdir64_r readdir_r
#define statvfs64 statvfs
#define stat64 stat
#define fstat64 fstat
#define lstat64 lstat

#endif /* __CYGWIN__ */

#endif /* CYGWIN_COMPAT_H */
