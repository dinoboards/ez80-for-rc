	.assume	adl=1
	section	.text_on_chip, "ax", @progbits
	.global	_sl_s16_s32_10

; extern int32_t sl_s16_s32_10(int16_t val);

; iy + 3  Low
; iy + 4  high
; iy + 5  N/A

_sl_s16_s32_10:
	ld	iy, 0				; reference stack frame
	add	iy, sp

	ld	l, 0
	ld	h, (iy+3)
	ld	a, (iy+4)
	ld	e, 0				; EAHL < val << 8

	sla	h
	rl	a
	rl	e

	sla	h
	rl	a
	rl	e

	ld	(iy+3), hl			; UHL <= EHL
	ld	(iy+5), a
	ld	hl, (iy+3)

	; result in aUHL
	ret
