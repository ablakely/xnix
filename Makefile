# xnix makefile
# Written by Aaron Blakely <aaron@ephasic.org>

CC = i586-elf-gcc
AS = nasm
LK = i586-elf-ld
OBJECTS = $(OBJDIR)/*.o
EXECTUABLE = ./kernel
CFLAGS = -m32 -fstrength-reduce -fno-builtin-puts -fno-builtin-printf -fno-builtin-function -finline-functions -nostdinc -fno-stack-protector -fomit-frame-pointer -nostdlib  $(IDIRS)
LDFLAGS = -Tetc/link.ld -melf_i386
ASFLAGS = -Werror -felf
IDIRS = -Ikernel/ -Ikernel/lib -Ikernel/io
OBJDIR = build/objects

all:
	@mkdir -p build
	@mkdir -p build/objects
	@echo "Compiling the kernel..."
	@$(CC) -m32 -c kernel/xnix.c $(CFLAGS) -o $(OBJDIR)/xnix.o
	@$(CC) -m32 -c kernel/io/iomem.c $(CFLAGS) -o $(OBJDIR)/iomem.o
	@$(CC) -m32 -c kernel/tty/console.c $(CFLAGS) -o $(OBJDIR)/xnixconsole.o
	@$(CC) -m32 -c kernel/lib/stdio.c $(CFLAGS) -o $(OBJDIR)/stdio.o
	@$(CC) -m32 -c kernel/lib/stdlib.c $(CFLAGS) -o $(OBJDIR)/stdlib.o
	@$(CC) -m32 -c kernel/cpu/IA32/gdt/gdt.c $(CFLAGS) -o $(OBJDIR)/gdtc.o
	@$(CC) -m32 -c kernel/cpu/IA32/idt/idt.c $(CFLAGS) -o $(OBJDIR)/idt.o
	@$(CC) -m32 -c kernel/cpu/IA32/isrs/isrs.c $(CFLAGS) -o $(OBJDIR)/isrs.o
	@$(CC) -m32 -c kernel/cpu/IA32/irqs/irqs.c $(CFLAGS) -o $(OBJDIR)/irqs.o
	@$(CC) -m32 -c kernel/cpu/IA32/descriptors.c $(CFLAGS) -o $(OBJDIR)/descriptors.o
	@$(CC) -m32 -c kernel/cpu/IA32/handlers.c $(CFLAGS) -o $(OBJDIR)/handlers.o
	@$(CC) -m32 -c kernel/io/kb/kb.c $(CFLAGS) -o $(OBJDIR)/kb.o
	@$(CC) -m32 -c kernel/io/kb/layouts/us/qwerty/map.c $(CFLAGS) -o $(OBJDIR)/qwerty.o
	@$(CC) -m32 -c kernel/io/pit/pit.c $(CFLAGS) -o $(OBJDIR)/pit.o
	@$(CC) -m32 -c kernel/tty/spinner.c $(CFLAGS) -o $(OBJDIR)/spinner.o
	@$(CC) -m32 -c kernel/lib/panic.c $(CFLAGS) -o $(OBJDIR)/panic.o
	@$(CC) -m32 -c kernel/mem/malloc.c $(CFLAGS) -o $(OBJDIR)/malloc.o
	@$(CC) -m32 -c kernel/mem/paging.c $(CFLAGS) -o $(OBJDIR)/paging.o

	@echo "Running the assembler..."
	@$(AS) $(ASFLAGS) kernel/cpu/IA32/boot/prep/head.s -o $(OBJDIR)/head.o
	@$(AS) $(ASFLAGS) kernel/cpu/IA32/gdt/gdt.s -o $(OBJDIR)/gdtasm.o
	@$(AS) $(ASFLAGS) kernel/cpu/IA32/idt/idt.s -o $(OBJDIR)/idtasm.o
	@$(AS) $(ASFLAGS) kernel/cpu/IA32/isrs/isrs.s -o $(OBJDIR)/isrsasm.o
	@$(AS) $(ASFLAGS) kernel/cpu/IA32/irqs/irqs.s -o $(OBJDIR)/irqsasm.o

	@echo "Running the linker..."
	@$(LK) $(LDFLAGS) -o build/kernel.bin $(OBJECTS)

	@echo "Build succuessful: kernel=build/kernel.bin"

test: all
	@kvm -kernel build/kernel.bin

github: all
	@rm build/kernel.bin
	@rm build/floppy.img
	@rm build/objects/*
	@git add --all
	git commit
	git push -u origin master

clean:
	@rm build/kernel.bin
	@rm build/floppy.img
	@rm build/objects/*
	@rm etc/blog.txt etc/snapshot.txt

floppy: all
	@mkdir tmp
	@dd if=/dev/zero of=tmp/pad bs=1 count=750
	@cat etc/stage1 etc/stage2 tmp/pad build/kernel.bin > build/floppy.img
	@rm -rf tmp
	@echo "Done: build/floppy.img"

bochs: floppy
	@cd etc;xterm bochs&
	@echo "Use kernel 200+`stat -c%b build/kernel.bin`"
