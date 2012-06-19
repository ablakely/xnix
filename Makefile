# xnix: build file
# Written by Aaron Blakely <aaron@ephasic.org>
#

CSOURCES	= console.c iomem.c mem/detec.c xnix.c cpu/IA32/*.c
ASOURCES	= cpu/IA32/*.s cpu/IA32/boot/*.s cpu IA32/boot/prep/*.s
OBJS		= $(patsubst %.s, %.o, $(SOURCES:%.c=%.o))
CS		= gcc
AS		= nasm
INCLUDEDIRS	= -I./kernel/lib -I./kernel/mem -I./kernel/cpu/IA32 -I./kernel/cpu/IA32/boot -I./kernel/tty
CFLAGS		= -m32 -Wall -02 -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-stack-pointer $(INCLUDEDIRS)
LDFLAGS		= -T./etc/link.ld -melf
ASFLAGS		= -felf

all: $(ASOURCES) $(CSOURCES) link

clean:
	@echo "cleaning build/"
	@-rm -rf build/obj/*.o kernel

link:
	@echo "linking..."
	@cd build/obj; ld $(LDFLAGS) -o ../kernel $(OBJS); cd ../..

%.s:
	@echo "running assembler: $*"
	@$(AS) $(ASFLAGS) -o build/obj/$*.o $(ASOURCES)

%.c:
	@echo "running compiler: $*"
	@$(CS) $(CFLAGS) -o build/obj/$*.o $(CSOURCES)

test:
	@echo "launching kvm..."
	@kvm -kernel build/kernel
