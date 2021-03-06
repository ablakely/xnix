; xnix - boot code
; sets up memory and calls the kernel's main loop
;
; Written by Aaron Blakely <aaron@ephasic.org>

[GLOBAL start]
[EXTERN code]
[EXTERN bss]
[EXTERN end]

; allow grub to boot this kernel
XBOOT_PAGE_ALIGN	equ 1<<0	; load kernel on a page boundary
XBOOT_MEM_INFO		equ 1<<1
XBOOT_HEADER_MAGIC	equ 0x1BADB002
XBOOT_HEADER_FLAGS	equ XBOOT_PAGE_ALIGN | XBOOT_MEM_INFO
XBOOT_CHECKSUM		equ -(XBOOT_HEADER_MAGIC + XBOOT_HEADER_FLAGS)

[BITS 32]				; 32 bit instructions
[GLOBAL mboot]				; allow xboot to be called from C.

mboot:
	dd XBOOT_HEADER_MAGIC		; GRUB will search for this value
					; on each 4-byte boundary

	dd XBOOT_HEADER_FLAGS		; Boot Flags
	dd XBOOT_CHECKSUM		; a checksum of the MAGIC and FLAGS

	dd mboot
	dd code
	dd bss
	dd end
	dd start

section .text

[GLOBAL start]
[EXTERN xnix_main]

start:
	push	esp
	push	ebx
	cli

	call xnix_main			; Enter Night.  Exit Light.  Take my hand...
					; Off to never-never land.  Start the kernel.
	cli				; disable interrupts

.hang:
	hlt
	jmp	$			; loop forever if the kernel exits to keep the CPU
					; from running whatever is left in the memory

