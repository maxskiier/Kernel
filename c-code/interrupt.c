#include "interrupt.h"

static idt_descriptor_t idt_descriptor;
static idt_entry_t idt[256];
static isr_handler_t isr_handlers[256] = { 0 };

void _lidt(idt_descriptor_t* desc) {
    __asm__ volatile ("lidt (%0)" : : "r"(desc));
}

void _idt_set_entry(uint8_t vec, void* handler) {
    uint32_t addr = (uint32_t)handler;

    idt[vec].addr_low = addr & 0xFFFF;
    idt[vec].segment = 0x08;
    idt[vec].res = 0x00;
    idt[vec].flags = 0x8E;
    idt[vec].addr_high = (addr >> 16) & 0xFFFF;
}

void register_handler(uint8_t vector, isr_handler_t handler) {
    isr_handlers[vector] = handler;
}

void sysint(uint32_t vector, uint32_t err) {
    if (isr_handlers[vector]) {
        interrupt_frame_t frame = { .vector = vector, .error_code = err };
        isr_handlers[vector](&frame);
    } else {
        panic("An interrupt without a handler was called.\n");
    }

    if (vector >= 0x20 && vector <= 0x2F) {
        _pic_eoi(vector - 0x20);
    }
}

void _idt_init(void) {
    idt_descriptor.limit = sizeof(idt) - 1;
    idt_descriptor.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++) {
        _idt_set_entry(i, isr_stub_table[i]);
    }

    _lidt(&idt_descriptor);
}
