#include "panic.h"
#include "syscall.h"

void __attribute__((noreturn)) panic(const char* why) {
    write(2, "\nKernel panicked!\n", 19);
    write(2, why, strlen(why));
    asm __volatile__ ("cli");
    while (1);
}