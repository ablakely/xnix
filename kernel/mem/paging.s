global readCR0 			; u32int readCR0()
global writeCR0 		; void writeCR0(u32int cr0)

readCR0:
	mov	%cr0, %eax
	ret

writeCR0:
	push	ebp
	mov	ebp, esp

	mov	8(%ebp), %eax
	mov	%eax, %cr0

	mov	esp, ebp
	pop	ebp
	ret

