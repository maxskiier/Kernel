# Makefile for building the myos kernel from the scripts directory.

ROOT := ..
SRCDIR := $(ROOT)/c-code
ASMDIR := $(ROOT)/asm
BUILDDIR := $(ROOT)/build
OUT := $(ROOT)/myos
ISO_BOOT_DIR := $(ROOT)/isodir/boot
ISO_TARGET := $(ISO_BOOT_DIR)/myos

CC := gcc
LD := ld
CFLAGS := -m32 -march=i386 -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -Wall -Wextra -I$(SRCDIR)
ASFLAGS := -m32 -c
LDFLAGS := -m elf_i386 -T $(ROOT)/scripts/linker.ld

C_SRCS := $(SRCDIR)/kernel.c $(SRCDIR)/stdin.c $(SRCDIR)/video32.c $(SRCDIR)/interrupt.c $(SRCDIR)/syscall.c
ASM_SRCS := $(ASMDIR)/boot.s $(ASMDIR)/intreg.s

C_OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst $(ASMDIR)/%.s,$(BUILDDIR)/%.o,$(ASM_SRCS))
OBJS := $(C_OBJS) $(ASM_OBJS)

.PHONY: all clean install debug
all: $(OUT)

debug: CFLAGS += -g
debug: ASFLAGS += -g
debug: $(OUT)

$(OUT): | $(BUILDDIR) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(ASMDIR)/%.s
	$(CC) $(ASFLAGS) $< -o $@

install: all | $(ISO_BOOT_DIR)
	cp $(OUT) $(ISO_TARGET)

$(ISO_BOOT_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILDDIR) $(OUT) $(ISO_TARGET)
