#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>
#include "pic.h"

typedef struct {
    uint32_t vector;
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} interrupt_frame_t;

typedef struct __attribute__((packed)) {
    uint16_t    addr_low;
    uint16_t    segment;
    uint8_t     res;
    uint8_t     flags;
    uint16_t    addr_high;
} idt_entry_t;

typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint32_t base;
} idt_descriptor_t;

typedef void (*isr_handler_t)(interrupt_frame_t *);

void _lidt(idt_descriptor_t* desc);
void _idt_set_entry(uint8_t vec, void* handler);
void register_handler(uint8_t vector, isr_handler_t handler);
void sysint(uint32_t vector, uint32_t err);
extern void *isr_stub_table[];
void _idt_init(void);

#endif /* INTERRUPT_H */