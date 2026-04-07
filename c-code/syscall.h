#ifndef SYSCALL_H
#define SYSCALL_H

#include <stddef.h>
#include <stdint.h>
#include "syshard.h"
#include "systypes.h"

ssize_t read(int fd, void *addr, size_t count);

ssize_t write(int fd, void *addr, size_t count);

ssize_t syscall(int fd, void *addr, size_t count);
#endif // SYSCALL_H