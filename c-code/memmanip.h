#ifndef MEMMANIP_H
#define MEMMANIP_H

#include <stddef.h>
#include <stdint.h>

// Regular memmove function for userspace
// memmove: memcpy but slower, and includes protections for overlapping memory
void* memmove(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    if (d < s) {
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = n; i > 0; i--) {
            d[i-1] = s[i-1];
        }
    }
    return dest;
}
// memcpy: memmove but faster, more dangerous as it assumes memory won't overlap
void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}
// kmemmove: memmove, but for kernelspace, includes volatile qualifiers
volatile void* kmemmove(volatile void* dest, volatile const void* src, size_t n) {
    volatile unsigned char* d = (volatile unsigned char*)dest;
    volatile const unsigned char* s = (volatile const unsigned char*)src;

    if (d < s) {
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = n; i > 0; i--) {
            d[i-1] = s[i-1];
        }
    }
    return dest;
}
// kmemcpy: memcpy, but for kernelspace, includes volatile qualifiers
volatile void* kmemcpy(volatile void* dest, volatile const void* src, size_t n) {
    volatile unsigned char* d = (volatile unsigned char*)dest;
    volatile const unsigned char* s = (volatile const unsigned char*)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}
#endif /* MEMMANIP_H */