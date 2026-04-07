#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "syshard.h"

#define PS2_DATA_PORT   0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64
#define PS2_IRQ         1

#define SCANCODE_RELEASE_MASK 0x80

#define STDIN_BUFFER_SIZE 0xFF

static void _stdin_buffer_push(char c);

static char _stdin_buffer_pop(void);

#define SC_LSHIFT_PRESS    0x2A
#define SC_RSHIFT_PRESS    0x36
#define SC_LSHIFT_RELEASE  0xAA
#define SC_RSHIFT_RELEASE  0xB6
#define SC_CTRL_PRESS      0x1D
#define SC_CTRL_RELEASE    0x9D
#define SC_CAPS_LOCK       0x3A

static char scancode_to_ascii(uint8_t scancode);

static void ps2_stdin_isr(interrupt_frame_t* frame);

void _stdin_init(void);

bool _stdin_has_char(void);

char _stdin_getch(void);

int getch(void);

/* Read one line into buf, up to max-1 chars, null-terminated.
 * Returns number of chars read (excluding null terminator), or -1 on error. */
int readline(char* buf, size_t max);

size_t strlen(const char* str);

#endif /* KEYBOARD_H */
