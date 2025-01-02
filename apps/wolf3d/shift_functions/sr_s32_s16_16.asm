	.assume	adl = 1
	section	.text_on_chip,"ax",@progbits
	.global	_sr_s32_s16_16

; extern int16_t sr_s32_s16_16(int32_t val);

; iy + 3  Low
; iy + 4
; iy + 5
; iy + 6  High

_sr_s32_s16_16:
	xor	a				; zero UHL
	sbc	hl, hl

	ld	iy, 0				; reference stack frame
	add	iy, sp

	ld	h, (iy+6)			; extract high part of
	ld	l, (iy+5)			; val

	ret
