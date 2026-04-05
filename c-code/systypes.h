#ifndef SYSTYPES_H
#define SYSTYPES_H

#include <stdint.h>

typedef intmax_t ssize_t;
typedef int32_t pid_t;
typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef uint32_t mode_t;
typedef uint32_t dev_t;
typedef uint32_t nlink_t;
typedef uint32_t off_t;
typedef uint32_t time_t;
typedef uint32_t clock_t;
typedef uint32_t suseconds_t;
typedef uint32_t blksize_t;
typedef uint32_t blkcnt_t;

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2
#endif // SYSTYPES_H