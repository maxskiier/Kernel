#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "systypes.h"
#include "keyboard.h"

void __attribute__((noreturn)) panic(const char* why);

#define abort(void) panic("Kernel aborted. It may be due to multiple things.\n1. An assertion's requirement wasn't met.\n2. An internal error occured.\n To fix, restart your computer.")