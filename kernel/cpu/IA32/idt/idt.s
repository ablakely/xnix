global idt_flush
extern idtp

idt_flush:
	mov eax, [esp+4]
	lidt [eax]
	ret

