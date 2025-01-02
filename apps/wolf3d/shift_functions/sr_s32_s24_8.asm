	.assume	adl = 1
	section	.text_on_chip,"ax",@progbits
	.global	_rr_s32_s24_8

; extern int24_t rr_s32_s24_8(int32_t val);

; iy + 3  Low
; iy + 4
; iy + 5
; iy + 6  High

_rr_s32_s24_8:
	ld	iy, 0				; reference stack frame
	add	iy, sp

	ld	hl, (iy+4)			; hl assigned to upper 24bits of val

	ret
