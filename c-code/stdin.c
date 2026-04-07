#include "keyboard.h"
#include "syscall.h"

static char     stdin_buffer[STDIN_BUFFER_SIZE];
static uint8_t  stdin_buffer_head = 0;
static uint8_t  stdin_buffer_tail = 0;

static bool   shift_pressed  =  false;
static bool   caps_lock_on   =  false;
static bool   ctrl_pressed   =  false;

static const char scancode_table_lower[] = {
/*00*/  0,    0,   '1', '2', '3', '4', '5', '6',
/*08*/ '7', '8', '9', '0', '-', '=',  '\b', '\t',
/*10*/ 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
/*18*/ 'o', 'p', '[', ']', '\n',  0,  'a', 's',
/*20*/ 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
/*28*/ '\'', '`',  0, '\\', 'z', 'x', 'c', 'v',
/*30*/ 'b', 'n', 'm', ',', '.', '/',  0,  '*',
/*38*/  0,  ' ',  0,   0,   0,   0,   0,   0,
};

static const char scancode_table_upper[] = {
/*00*/  0,    0,   '!', '@', '#', '$', '%', '^',
/*08*/ '&', '*', '(', ')', '_', '+',  '\b', '\t',
/*10*/ 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
/*18*/ 'O', 'P', '{', '}', '\n',  0,  'A', 'S',
/*20*/ 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
/*28*/ '"', '~',  0,  '|', 'Z', 'X', 'C', 'V',
/*30*/ 'B', 'N', 'M', '<', '>', '?',  0,  '*',
/*38*/  0,  ' ',  0,   0,   0,   0,   0,   0,
};

#define SCANCODE_TABLE_SIZE (sizeof(scancode_table_upper) / sizeof(scancode_table_lower[0]))

static inline bool stdin_buffer_empty(void) { return stdin_buffer_head == stdin_buffer_tail; }
static inline bool stdin_buffer_full(void) {
    return ((stdin_buffer_head + 1) % STDIN_BUFFER_SIZE) == stdin_buffer_tail;
}

static void _stdin_buffer_push(char c) {
    if (!stdin_buffer_full()) {
        stdin_buffer[stdin_buffer_head] = c;
        stdin_buffer_head = (stdin_buffer_head + 1) % STDIN_BUFFER_SIZE;
    }
}

static char _stdin_buffer_pop(void) {
    char c = stdin_buffer[stdin_buffer_tail];
    stdin_buffer_tail = (stdin_buffer_tail + 1) % STDIN_BUFFER_SIZE;
    return c;
}


char scancode_to_ascii(uint8_t scancode) {
    if (scancode & SCANCODE_RELEASE_MASK)
        return 0;

    if (scancode >= SCANCODE_TABLE_SIZE)
        return 0;
    
    bool use_upper = shift_pressed ^ caps_lock_on;
    return use_upper ? scancode_table_upper[scancode]
                     : scancode_table_lower[scancode];
}

static void ps2_stdin_isr(interrupt_frame_t* frame) {
    (void)frame;
    uint8_t scancode = inb(PS2_DATA_PORT);

    switch (scancode) {
        case SC_LSHIFT_PRESS:
        case SC_RSHIFT_PRESS:   shift_pressed = true;           break;
        case SC_LSHIFT_RELEASE:
        case SC_RSHIFT_RELEASE: shift_pressed = false;          break;
        case SC_CTRL_PRESS:     ctrl_pressed = true;            break;
        case SC_CTRL_RELEASE:   ctrl_pressed = false;           break;
        case SC_CAPS_LOCK:      caps_lock_on = !caps_lock_on;  break;

        default: {
            char c = scancode_to_ascii(scancode);
            if (c) _stdin_buffer_push(c);
            break;
        }
    }
}

void _stdin_init(void) {
    // Try minimal PS2 keyboard enable for VM compatibility
    // Wait for input buffer to be empty
    while (inb(PS2_STATUS_PORT) & 0x2);
    // Enable keyboard interface
    outb(PS2_COMMAND_PORT, 0xAE);
    // Wait for input buffer to be empty again
    while (inb(PS2_STATUS_PORT) & 0x2);
    // Send reset command to keyboard
    outb(PS2_DATA_PORT, 0xFF);
    // Wait a bit and drain any response
    for (volatile int i = 0; i < 100000; i++);
    
    int drain_count = 0;
    while ((inb(PS2_STATUS_PORT) & 0x1) && drain_count < 10) {
        inb(PS2_DATA_PORT);
        drain_count++;
    }
    
    // Register the keyboard interrupt handler
    register_handler(0x21, ps2_stdin_isr);
    
    // Unmask IRQ1 (keyboard)
    _unmask_irq(PS2_IRQ);
}


bool _stdin_has_char(void) {
    return !stdin_buffer_empty();
}

extern void terminal_putchar(char c);
char _stdin_getch(void) {
    while (stdin_buffer_empty());
    char c = _stdin_buffer_pop();
    terminal_putchar(c);
    return c;
}

// User friendly wrapper for _stdin_getch()
// Obtains exactly one readable ASCII character
int getch(void) {
    return _stdin_getch();
}

/* Read one line into buf, up to max-1 chars, null-terminated.
 * Returns number of chars read (excluding null terminator). */
int readline(char* buf, size_t max) {
    if (!buf || max <= 0)
        return -1;
    
    int count = 0;
    while (count < max - 1) {
        char c = _stdin_getch();
        
        if (c == '\n') {
            buf[count] = '\0';
            return count;
        }
        
        if (c == '\b' || c == 0x7F) {
            if (count > 0) {
                count--;
                terminal_putchar(' ');
                terminal_putchar('\b');
            }
            continue;
        }
        
        buf[count++] = c;
    }
    
    buf[count] = '\0';
    return count;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}