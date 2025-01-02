	.assume	adl = 1
	section	.text_on_chip,"ax",@progbits
	.global	_sr_s32_s32_6

; extern int24_t sr_s32_s32_6(int32_t val);

; iy + 3  Low
; iy + 4
; iy + 5
; iy + 6  High

_sr_s32_s32_6:
	ld	iy, 0				; reference stack frame
	add	iy, sp

	ld	hl, (iy+3)
	ld	a, (iy+5)			; u of hl
	ld	e, (iy+6)			; high byte of val

	sra	e				; val >> 1
	rra
	rr	h
	rr	l

	sra	e				; val >> 1
	rra
	rr	h
	rr	l

	sra	e				; val >> 1
	rra
	rr	h
	rr	l

	sra	e				; val >> 1
	rra
	rr	h
	rr	l

	sra	e				; val >> 1
	rra
	rr	h
	rr	l

	sra	e				; val >> 1
	rra
	rr	h
	rr	l

	ld	(iy+3), hl			; update u of hl
	ld	(iy+5), a
	ld	hl, (iy+3)

	ret
