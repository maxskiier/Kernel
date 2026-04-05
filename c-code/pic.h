#ifndef PIC_H
#define PIC_H

#include <stdint.h>
#include <stddef.h>
#include "portio.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define EOI 0x20

static inline void _pic_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, EOI);
    }
    outb(PIC1_COMMAND, EOI);
}

#undef EOI

static inline void _pic_init(void) {
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

static inline void _unmask_irq(uint8_t irq) {
    uint16_t port;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
    }

    uint8_t mask = inb(port);
    mask &= ~(1 << irq);
    outb(port, mask);
}

static inline void _mask_irq(uint8_t irq) {
    uint8_t port;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
    }

    uint8_t mask = inb(port);
    mask |= (1 << irq);
    outb(port, mask);
}

#undef PIC1_COMMAND
#undef PIC1_DATA
#undef PIC2_COMMAND
#undef PIC2_DATA
#endif