global	_isr0
global	_isr1
global	_isr2
global	_isr3
global	_isr4
global	_isr5
global	_isr6
global	_isr7
global	_isr8
global	_isr9
global	_isr10
global	_isr11
global	_isr12
global	_isr13
global	_isr14
global	_isr15
global	_isr16
global	_isr17
global	_isr18
global	_isr19
global	_isr20
global	_isr21
global	_isr22
global	_isr23
global	_isr24
global	_isr25
global	_isr26
global	_isr27
global	_isr28
global	_isr29
global	_isr30
global	_isr31
global	_isr128

_isr0:
	cli
	push	byte 0
	push	0
	jmp	isr_common_stub

_isr1:
	cli
	push	byte 0
	push	1
	jmp	isr_common_stub

_isr2:
	cli
	push	byte 0
	push	2
	jmp	isr_common_stub

_isr3:
	cli
	push	byte 0
	push	3
	jmp	isr_common_stub

_isr4:
	cli
	push	byte 0
	push	4
	jmp	isr_common_stub

_isr5:
	cli
	push	byte 0
	push	5
	jmp	isr_common_stub

_isr6:
	cli
	push	byte 0
	push	6
	jmp	isr_common_stub

_isr7:
	cli
	push	byte 0
	push	7
	jmp	isr_common_stub

_isr8:
	cli
	push	8
	jmp	isr_common_stub

_isr9:
	cli
	push	byte 0
	push	9
	jmp	isr_common_stub

_isr10:
	cli
	push	10
	jmp	isr_common_stub

_isr11:
	cli
	push	11
	jmp	isr_common_stub

_isr12:
	cli
	push	12
	jmp	isr_common_stub

_isr13:
	cli
	push	13
	jmp	isr_common_stub

_isr14:
	cli
	push	14
	jmp	isr_common_stub

_isr15:
	cli
	push	byte 0
	push	15
	jmp	isr_common_stub

_isr16:
	cli
	push	byte 0
	push	16
	jmp	isr_common_stub

_isr17:
	cli
	push	byte 0
	push	17
	jmp	isr_common_stub

_isr18:
	cli
	push	byte 0
	push	18
	jmp	isr_common_stub

_isr19:
	cli
	push	byte 0
	push	19
	jmp	isr_common_stub

_isr20:
	cli
	push	byte 0
	push	20
	jmp	isr_common_stub

_isr21:
	cli
	push	byte 0
	push	21
	jmp	isr_common_stub

_isr22:
	cli
	push	byte 0
	push	22
	jmp	isr_common_stub

_isr23:
	cli
	push	byte 0
	push	23
	jmp	isr_common_stub

_isr24:
	cli
	push	byte 0
	push	24
	jmp	isr_common_stub

_isr25:
	cli
	push	byte 0
	push	25
	jmp	isr_common_stub

_isr26:
	cli
	push	byte 0
	push	26
	jmp	isr_common_stub

_isr27:
	cli
	push	byte 0
	push	27
	jmp	isr_common_stub

_isr28:
	cli
	push	byte 0
	push	28
	jmp	isr_common_stub

_isr29:
	cli
	push	byte 0
	push	29
	jmp	isr_common_stub

_isr30:
	cli
	push	byte 0
	push	30
	jmp	isr_common_stub

_isr31:
	cli
	push	byte 0
	push	31
	jmp	isr_common_stub

_isr128:
	cli
	push	byte 0
	push	128
	jmp	isr_common_stub

extern isr_handler

isr_common_stub:
	pusha

	mov	ax, ds
	push	eax

	mov	ax, 0x10
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

	call	isr_handler

	pop	eax
	mov	ds, bx
	mov	es, bx
	mov 	fs, bx
	mov	gs, bx

	popa
	add	esp, 8

	sti
	iret
