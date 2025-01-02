	.assume	adl = 1
	section	.text_on_chip,"ax",@progbits
	.global	_fixed_rounded_down

; extern int16_t fixed_rounded_down(fixed a);

; iy + 3  Low
; iy + 4
; iy + 5
; iy + 6  High

_fixed_rounded_down:
	ld	iy, 0
	add	iy, sp

	xor	a
	sbc	hl, hl

	ld	h, (iy+6)
	ld	l, (iy+5)

	ret
