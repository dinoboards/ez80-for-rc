	.assume	adl=1

	section	.text, "ax", @progbits
	.global	_putchar

; override putchat to bypass cp/m
; and always send via default serial (not CRT)

; int putchar(int character);
_putchar:
	pop	hl ; return address
	pop	de ; character
	push	de
	push	hl

	ld	b, 1
	ld	c, $80
	jp 	hbios_adl
