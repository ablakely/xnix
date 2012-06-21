# xnix makefile
# Written by Aaron Blakely <aaron@ephasic.org>

CC = gcc
OBJECTS = $(OBJDIR)/*.o
EXECTUABLE = ./kernel
CFLAGS = -m32 -fstrength-reduce -fno-builtin-puts -fno-builtin-printf -fno-builtin-function -finline-functions -nostdinc -fno-stack-protector -fomit-frame-pointer -nostdlib  $(IDIRS)
LDFLAGS = -Tetc/link.ld
ASFLAGS = -Werror -felf
IDIRS = -Ikernel/ -Ikernel/lib -Ikernel/io
OBJDIR = build/objects

all:
	@mkdir -p build
	@mkdir -p build/objects
	@echo "Compiling the kernel..."
	@gcc -m32 -c kernel/xnix.c $(CFLAGS) -o $(OBJDIR)/xnix.o
	@gcc -m32 -c kernel/io/iomem.c $(CFLAGS) -o $(OBJDIR)/iomem.o
	@gcc -m32 -c kernel/tty/console.c $(CFLAGS) -o $(OBJDIR)/xnixconsole.o
	@gcc -m32 -c kernel/lib/stdio.c $(CFLAGS) -o $(OBJDIR)/stdio.o
	@gcc -m32 -c kernel/lib/stdlib.c $(CFLAGS) -o $(OBJDIR)/stdlib.o
	@gcc -m32 -c kernel/cpu/IA32/gdt/gdt.c $(CFLAGS) -o $(OBJDIR)/gdtc.o
	@gcc -m32 -c kernel/cpu/IA32/idt/idt.c $(CFLAGS) -o $(OBJDIR)/idt.o
	@gcc -m32 -c kernel/cpu/IA32/isrs/isrs.c $(CFLAGS) -o $(OBJDIR)/isrs.o
	@gcc -m32 -c kernel/cpu/IA32/irqs/irqs.c $(CFLAGS) -o $(OBJDIR)/irqs.o
	@gcc -m32 -c kernel/io/kb/kb.c $(CFLAGS) -o $(OBJDIR)/kb.o
	@gcc -m32 -c kernel/io/kb/layouts/us/qwerty/map.c $(CFLAGS) -o $(OBJDIR)/qwerty.o
	@gcc -m32 -c kernel/io/pit/pit.c $(CFLAGS) -o $(OBJDIR)/pit.o
	@gcc -m32 -c kernel/tty/spinner.c $(CFLAGS) -o $(OBJDIR)/spinner.o
	@gcc -m32 -c kernel/lib/panic.c $(CFLAGS) -o $(OBJDIR)/panic.o

	@echo "Running the assembler..."
	@nasm $(ASFLAGS) kernel/cpu/IA32/boot/prep/head.s -o $(OBJDIR)/head.o
	@nasm $(ASFLAGS) kernel/cpu/IA32/gdt/gdt.s -o $(OBJDIR)/gdtasm.o
	@nasm $(ASFLAGS) kernel/cpu/IA32/idt/idt.s -o $(OBJDIR)/idtasm.o
	@nasm $(ASFLAGS) kernel/cpu/IA32/isrs/isrs.s -o $(OBJDIR)/isrsasm.o
	@nasm $(ASFLAGS) kernel/cpu/IA32/irqs/irqs.s -o $(OBJDIR)/irqsasm.o
	@echo "Running the linker..."
	@ld $(LDFLAGS) -o build/kernel.bin $(OBJECTS)

	@echo "Build succuessful: kernel=build/kernel.bin"

test: all
	@kvm -kernel build/kernel.bin

github: all
	@rm build/kernel.bin
	@rm build/objects/*
	@git add --all
	git commit
	git push -u origin master
