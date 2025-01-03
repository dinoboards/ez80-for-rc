	.assume	adl = 1
	section	.text_on_chip,"ax",@progbits
	.global	_sr_u24_u24_3

; extern uint24_t sr_u24_u24_3(uint24_t val);

; iy + 3  Low
; iy + 4  High
; iy + 5  Upper

_sr_u24_u24_3:
	ld	iy, 0				; reference stack frame
	add	iy, sp

	ld	hl, (iy+3)
	ld	a, (iy+5)			; u of hl

	srl	a				; val >> 1
	rr	h
	rr	l

	srl	a				; val >> 1
	rr	h
	rr	l

	srl	a				; val >> 1
	rr	h
	rr	l

	ld	(iy+3), hl			; update u of hl
	ld	(iy+5), a
	ld	hl, (iy+3)

	ret
