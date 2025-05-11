

; Multiplies HL by BC and returns the 16-bit product hl.
; corrupts DE
.macro	MUL_16_HL_BC
	ld	d, h
	ld	e, c
	mlt	de
	ld	d, l
	ld	h, b
	mlt	hl
	add	hl, de
	ld	h, l
	ld	l, 0
	ld	e, c
	mlt	de
	add	hl, de
.endm

.macro	LNEG_EUBC_AUHL
	xor	a, a	; A=0
	sbc	hl, hl	; UHL=0
	sbc	hl, bc	; UHL=-UBC
	sbc	a, e	; AUHL=-EUBC
.endm

; E/AUHL = EUHL + AUBC
.macro LADD_EUHL_AUBC
	add	hl, bc
	adc	a, e
	ld	e, a
.endm

; Compare HL, DE, by subtracting DE from HL
; destroys A and HL
; JP P, de > HL aka hl < de
; JP M, de <= hl aka hl >= de
.macro compare_16bit_signed
	or	a, a
	sbc.s	hl, de

	jp	pe, 1f

	ld	a, h
	rra
	xor	$40
	scf
	adc	a, a

1:
	; jp p, de_larger_than_hl
	; otherwise, de <= hl
.endm
