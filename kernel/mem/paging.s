[global enable_paging]

enable_paging:
	mov	eax, [ebp+8]
	mov	cr3, eax
	mov	eax, cr0

	or	eax, 0x80000000
	mov	cr0, eax
