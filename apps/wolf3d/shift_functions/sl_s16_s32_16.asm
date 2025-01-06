	.assume	adl=1
	section	.text_on_chip, "ax", @progbits
	.global	_sl_s16_s32_16

; extern int32_t sl_s16_s32_16(int16_t val);

; iy + 3  Low
; iy + 4  high
; iy + 5  N/A

_sl_s16_s32_16:
	ld	iy, 0				; reference stack frame
	add	iy, sp

	ld	hl, (iy+1)			; u <= iy+3
	ld	e, (iy+4)			; e <= iy+4

	ld	h, 0
	ld	l, 0
	ret
