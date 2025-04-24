	.assume	adl=1

	section	.text, "ax", @progbits
	.global	_putchar

; int putchar(int character);
_putchar:
	pop	hl ; return address
	pop	de ; character
	push	de
	push	hl

	ld	a, 3
	ld	b, 1
	RST.L	$10
	RET
