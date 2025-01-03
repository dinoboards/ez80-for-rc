	.assume	adl = 1
	section	.text_on_chip,"ax",@progbits
	.global	_sr_s32_s32_8

; extern int32_t sr_s32_s32_8(int32_t val);

; iy + 3  Low
; iy + 4
; iy + 5
; iy + 6  High

_sr_s32_s32_8:
	ld	iy, 0				; reference stack frame
	add	iy, sp

	ld	hl, (iy+4)			; hl assigned to upper 24bits of val

	; extend sign bit on high byte
	ld	e, (iy+6)

	sra	e
	sra	e
	sra	e
	sra	e
	sra	e
	sra	e
	sra	e
	sra	e

	ret
