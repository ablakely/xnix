[GLOBAL Entry16]

idt_real:
	dw 	0x3ff			; 256 entries, 4b each = 1K
	dd	0			; Real Mode IVT @ 0x0000

savcr0:
	dd	0			; Storage location for pmode CR0.

Entry16:
					; We're already in 16bit mode here.
	cli				; Disable interrupts.

	; Need 16-bit Protected Mode GDT entries!
	mov	eax, DATASEL16		; 16-bit Protected Mode data selector.
	mov	ds, eax
	mov	es, eax
	mov	fs, eax
	mov	gs, eax

	; Disable paging (we need everything to be 1:1 mapped).
	mov	eax, cr0
	mov	[savcr0], eax		; save protected mode
	and	eax, 0x7FFFFFFe		; Disable paging bit & enable 16-bit
					; protected mode.
	mov	cr0, eax

	jump	0:GoRMode		; Preform Far Jumpt to set CS

GoRMode:
	mov	sp, 0x8000		; pick a stack pointer.
	mov	ax, 0			; reset segment registers to 0.
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	lidt	[idt_real]
	sti				; restore interrupts
					; careful: unhandled ints will kill it

