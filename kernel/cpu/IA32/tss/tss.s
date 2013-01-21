[GLOBAL tss_flush]
tss_flush:
	mov	ax, 0x2B	; load the index of our TSS structure

	ltr	ax		; load into the task state register
	ret
