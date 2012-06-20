# xnix makefile
# Written by Aaron Blakely <aaron@ephasic.org>

CC = gcc
OBJECTS = $(OBJDIR)/*.o
EXECTUABLE = build/kernel.bin
CFLAGS = -m32 -fstrength-reduce -fno-builtin-puts -fno-builtin-printf -fno-builtin-function -finline-functions -nostdinc -fno-stack-protector -fomit-frame-pointer -nostdlib  $(IDIRS)
LDFLAGS = -Tetc/link.ld -melf_i386
ASFLAGS = -felf
IDIRS = -Ikernel/ -Ikernel/lib -Ikernel/io
OBJDIR = build/objects

all:
	@mkdir -p build
	@mkdir -p build/objects
	@echo "Compiling the kernel..."
	@gcc -m32 -c kernel/xnix.c $(CFLAGS) -o $(OBJDIR)/xnix.o
	@gcc -m32 -c kernel/io/iomem.c $(CFLAGS) -o $(OBJDIR)/iomem.o
	@gcc -m32 -c kernel/tty/console.c $(CFLAGS) -o $(OBJDIR)/console.o
	@gcc -m32 -c kernel/cpu/IA32/gdt/gdt.c $(CFLAGS) -o $(OBJDIR)/gdt.o
	@gcc -m32 -c kernel/cpu/IA32/idt/idt.c $(CFLAGS) -o $(OBJDIR)/idt.o
	@gcc -m32 -c kernel/cpu/IA32/isrs/isrs.c $(CFLAGS) -o $(OBJDIR)/isrs.o
	@gcc -m32 -c kernel/cpu/IA32/irqs/irqs.c $(CFLAGS) -o $(OBJDIR)/irqs.o
	@gcc -m32 -c kernel/io/kb/kb.c $(CFLAGS) -o $(OBJDIR)/kb.o
	@echo "Running the assembler..."
	@nasm $(ASFLAGS) kernel/cpu/IA32/boot/prep/head.s -o $(OBJDIR)/head.o
	@nasm $(ASFLAGS) kernel/cpu/IA32/gdt/gdt.s -o $(OBJDIR)/gdtasm.o
	@nasm $(ASFLAGS) kernel/cpu/IA32/idt/idt.s -o $(OBJDIR)/idtasm.o
	@nasm $(ASFLAGS) kernel/cpu/IA32/isrs/isrs.s -o $(OBJDIR)/isrsasm.o
	@nasm $(ASFLAGS) kernel/cpu/IA32/irqs/irqs.s -o $(OBJDIR)/irqsasm.o
	@echo "Running the linker..."
	@ld $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS)

debug:
	CFLAGS = "-Wall $(CFLAGS)"
	all

clean:
	@rm -rf build/
