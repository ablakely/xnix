[GLOBAL copy_page_physical]
copy_page_physical:
	push		ebx			; Preserve the contents of EBX.
	pushf					; push flags
						;
	cli					; disable interrupts
						;
	mov 		ebx, [esp+12]		; source addr
	mov 		ecx, [esp+16]		; destination addr
						;
	mov 		edx, cr0		; get the control register...
	and 		edx, 0x7fffffff		;
	mov 		cr0, edx		; disable paging.
						;
	mov 		edx, 1024		; 1024*4bytes = 4096 bytes to copy

.loop:
	mov		eax, [ebx]		; get the word at the source address
	mov		[ecx], eax		; store it at dest addr
	add		ebx, 4			;
	add		ecx, 4			;
	dec		edx			;
	jnz		.loop			;
						;
	mov		edx, cr0		;
	or		edx, 0x80000000		; re-enable paging
	mov		cr0, edx		;
						;
	popf					; pop EFLAGS
	pop		ebx			; restore EBX
	ret
