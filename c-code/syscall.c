#include "syscall.h"



extern int terminal_write(const char* data, size_t size);
extern int readline(char* buf, size_t max);

typedef int (*syscall_handler_t)(void *memloc, size_t count);

static syscall_handler_t _syscall_pointer_array[] = {
    (syscall_handler_t) readline, // stdin
    (syscall_handler_t) terminal_write, // stdout
    (syscall_handler_t) terminal_write // stderr
};

ssize_t read(int fd, void *memloc, size_t count) {
    if (!(ssize_t) _syscall_pointer_array[fd])
        return -1;
    (ssize_t) _syscall_pointer_array[fd](memloc, count);
    return 0;
}

ssize_t write(int fd, void *memloc, size_t count) {
    if (!(ssize_t) _syscall_pointer_array[fd])
        return -1;
    (ssize_t) _syscall_pointer_array[fd](memloc, count);
    return 0;
}

ssize_t syscall(int fd, void *memloc, size_t count) {

}