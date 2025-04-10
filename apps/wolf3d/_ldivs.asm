; (c) Copyright 2001-2008 Zilog, Inc.
;-------------------------------------------------------------------------
; Long Division Signed
; Input:
;	Operand1:
;		  eHL : 32 bits
;
;	Operand2:
;		  aBC :	 32 bits
; Output:
;	Result:     : 24 bits
;
; Registers Used:
;
;-------------------------------------------------------------------------
	.assume	adl=1
	.global	__ldivs
	section	.text_on_chip, "ax", @progbits


.macro lneg
	push	af
	push	de
	ex	de, hl
	xor	a, a
	sbc	hl, hl
	sbc	hl, de
	pop	de
	sbc	a, e
	ld	e, a
	pop	af
.endm


__ldivs:
	push	bc
	push	af
	push	de
	bit	7, a
	jr	z, _L1
	push	hl
	or	a, a		; CR 7885
	sbc	hl, hl
	sbc	hl, bc
	adc	a, 0
	neg
	push	hl
	pop	bc
	pop	hl
_L1:
	bit	7, e
	jr	z, _L2
	lneg
_L2:

; __ldivu:
	push	ix
	push	iy

; Input:
;	EuHL = dividend
;	AuBC = divisor
; Output:
;	EuIX = quotient
;	AuHL = remainder
; __ldvrmu:
	push	hl
	pop	ix
	ld	iyh, a
	ld	iyl, 32
	xor	a, a
	sbc	hl, hl
_loop:
	add	ix, ix		;shift AuHLEuIX left
	rl	e
	adc	hl, hl
	rla
	sbc	hl, bc
	sbc	a, iyh
	jr	nc, _LA1
	add	hl, bc
	adc	a, iyh
	jr	_LA2
_LA1:
	inc	ix
_LA2:
	dec	iyl
	jr	nz, _loop


	ld	a, iyh
	push	ix
	pop	hl
	pop	iy
	pop	ix


	pop	bc
	pop	af
	ld	b, a
	xor	a, c
	jp	p, _LA3
	lneg
_LA3:
	ld	a, b
	pop	bc
	ret

