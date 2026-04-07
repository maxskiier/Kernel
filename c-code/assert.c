#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "systypes.h"

extern ssize_t write(int fd, void *addr, size_t len);
extern size_t strlen(const char* str);
void __attribute__((noreturn)) panic(const char* why) {
    write(2, "\nKernel panicked!\n", 19);
    write(2, why, strlen(why));
    nmi_loop:
    asm __volatile__ ("cli\n\t"
                      "hlt\n\t"
                       "jmp $nmi_loop");
}

void __attribute__((noreturn)) abort(void) {
    panic("An assertion's requirement wasn't met.\n");
}