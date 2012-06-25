global idt_flush
extern idtp

idt_flush:
	lidt [idtp]
	ret

