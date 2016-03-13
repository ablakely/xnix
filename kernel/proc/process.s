[GLOBAL read_eip]
[GLOBAL perform_task_switch]

read_eip:
	pop	eax
	jmp	eax

perform_task_switch:
	cli
	mov ecx, [esp+4]	; eip
	mov eax, [esp+8]	; physicall address of current directory
	mov ebp, [esp+12]	; ebp
	mov esp, [esp+16]	; esp
	mov cr3, eax 			; set the page directory
	mov eax, 0x12345 	; magic number to detect task switch
	sti
	jmp ecx
