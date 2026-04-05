#ifndef SYSHARD_H
#define SYSHARD_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "interrupt.h"

/* static void pit_isr(struct interrupt_frame* frame) {

} */

static inline void sleep(uint16_t mill) {
    (void)mill;
}

#endif /* SYSHARD_H */

