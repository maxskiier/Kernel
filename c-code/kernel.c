#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "syshard.h"
#include "portio.h"
#include "pic.h"
#include "interrupt.h"
#include "systypes.h"
#include "video.h"
#include "keyboard.h"
#include "syscall.h"



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