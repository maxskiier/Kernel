#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "syshard.h"
#include "portio.h"
#include "pic.h"
#include "interrupt.h"
#include "systypes.h"

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
extern volatile uint16_t* terminal_buffer;

extern void terminal_initialize(void);
extern void terminal_scroll(void);
extern void terminal_setcolor(uint8_t color);
extern void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
extern void terminal_putchar(char c);
extern void terminal_write(const char* data, size_t size);
extern void terminal_writestring(const char* data);

extern ssize_t read(int fd, void *addr, size_t count);
extern ssize_t write(int fd, void *addr, size_t count);

extern void _stdin_init(void);
extern bool _stdin_has_char(void);
extern char _stdin_getch(void);
extern int getch(void);
extern char* readline(char* buf, size_t max);

void __attribute__((noreturn)) kernel_main(void) 
{
	// Initialize terminal first
	terminal_initialize();
	
	_pic_init();
	_idt_init();
	_stdin_init();
	__asm__ volatile ("sti");
	
	// Test of the write syscall
	write(FD_STDOUT, "\nCongratulations, you've booted the machine. Now type a string.\n", sizeof(char[65]));
	char tmpbuf[256];
	
    while (1) {
        read(FD_STDIN, tmpbuf, sizeof(tmpbuf));
		write(FD_STDOUT, tmpbuf, strlen(tmpbuf)); // Let the user type whatever they want, we don't need to do anything else right now
    }
}