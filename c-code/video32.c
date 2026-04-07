#include "video.h"
#include "memmanip.h"
#include "portio.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
volatile uint16_t* terminal_buffer = (volatile uint16_t*)VGA_MEMORY;

#define VGA_CURSOR_CTRL_PORT 0x3D4
#define VGA_CURSOR_ADDR_PORT 0x3D5
#define VGA_SET_CURSOR_HI 0x0E
#define VGA_SET_CURSOR_LO 0x0F

void _cursor_setpos(uint16_t addr) {
	// Split the 16-bit address word, as we need to write the high byte to one VGA register and the low byte to another
	uint8_t hi = (addr >> 8) & 0x00FF;
	uint8_t lo = addr & 0x00FF;

	// Tell the VGA hardware we're setting the high byte of the cursor location
	outb(VGA_CURSOR_CTRL_PORT, VGA_SET_CURSOR_HI);
	// Send the high byte first
	outb(VGA_CURSOR_ADDR_PORT, hi);
	// Tell VGA hardware that we're now setting the low byte
	outb(VGA_CURSOR_CTRL_PORT, VGA_SET_CURSOR_LO);
	// Send the low byte
	outb(VGA_CURSOR_ADDR_PORT, lo);
}

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	_cursor_setpos(_CALCULATE_CURSOR_POS(terminal_row, terminal_column));
}

void terminal_scroll(void) {
	kmemmove( // Shift array to accomodate scroll
		terminal_buffer,
		terminal_buffer + VGA_WIDTH,
		(VGA_HEIGHT-1) * VGA_WIDTH * sizeof(uint16_t)
	);

	for (size_t x = 0; x < VGA_WIDTH; x++)
		terminal_buffer[(VGA_HEIGHT-1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);

	terminal_row = VGA_HEIGHT - 1;
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

// Made because the default terminal_putentryat won't support the proper cursor position setting for backspaces
void _put_backspace(uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	_cursor_setpos(index);
	terminal_buffer[index] = vga_entry(' ', color);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	_cursor_setpos(index+1);
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) 
{
	if (c == '\n') {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_scroll();
		_cursor_setpos(_CALCULATE_CURSOR_POS(terminal_row, terminal_column)-2);
		return;
	}

	if (c == '\b' || c == 0x7F) {
		if (terminal_column > 0) {
			terminal_column--;
			_put_backspace(terminal_color, terminal_column, terminal_row);
		} else if (terminal_row > 0) {
			terminal_row--;
			terminal_column = VGA_WIDTH - 1;
			_put_backspace(terminal_color, terminal_column, terminal_row);
		}
		return;
	}

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_scroll();
	}
}

int terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
	return 0;
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}