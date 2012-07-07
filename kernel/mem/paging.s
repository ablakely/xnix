[global enable_paging]
[global read_cr0]

enable_paging:
	mov	eax, [esp]
	mov	cr3, eax
	mov	eax, cr0

	or	eax, 0x80000000
	mov	cr0, eax

read_cr0:
	mov	eax, cr0
	retn
