	section	.text_on_chip, "ax", @progbits
	.assume	adl=1
	global	_FixedMul
	global	_mul_euhl_aubc
	extern	__lmulu

; typedef int32_t fixed;
;
; fixed FixedMul (fixed a, fixed b)
; IY + 3 = a
; IY + 9 = b
;
;   0     1       2       3      4    5
; 0...7 8...15 16...23 24...31  xxx  xxx
_FixedMul:
	ld	iy, 0
	add	iy, sp

	ld	hl, (iy+4)      ; a >> 8
	ld	bc, (iy+10)     ; b >> 8

	; shift sign of e and a
	ld	a, (iy+6)
	rlc	a
	sbc	a, a
	ld	e, a

	ld	a, (iy+12)
	rlc	a
	sbc	a, a

_mul_euhl_aubc:

; Multiplies EUHL by AUBC and returns the 32-bit product euhl.
;    I: AUBC=multiplier, EUHL=multiplicand, ADL=1
;    O: euhl=EUHL*AUBC
;   FO: --
;   CC: 93*r(PC)+15*w(SPL)+15*r(SPL)+41
;				; a		ude		uhl		uy		u(sp)
Mul_EUHL_AUBC_EUHL:		;--------------+---------------+---------------+---------------+----------------------------------------
	push	af		;								u(sp)=AF
	push	de		;									u(sp)=UDE
	ld	d, c		;		 de=CE
	mlt	de		;		ude=E*C
	ld	d, a		;		 d =A
	ld	a, e		; a=E*C
	ld	e, l		; 		 de=AL
	ld	iyl, e		;						 iyl=L
	mlt	de		;		ude=L*A
	add	a, e		; a=E*C+L*A
	dec	sp
	push	bc
	inc	sp
	inc	sp
	push	hl
	inc	sp
	inc	sp		;											 (sp)=[BCU][HLU]
	ld	d, h		;		 d =H
	ld	iyh, d		;						 iy=HL
	pop	hl		;				 hl=[BCU][HLU]						--
	ld	e, h		;		 de=H[BCU]
	mlt	de		;		ude=H*BCU
	add	a, e		; a=E*C+H*BCU+L*A
	ld	d, b		;		 d =B
	ld	e, l		;		 de=B[HLU]
	mlt	de		;		ude=HLU*B
	add	a, e		; a=E*C+HLU*B+H*BCU+L*A
	ld	d, h		;		 d =BCU
	ld	e, iyl		;		 de=[BCU]L
	ld	h, c		;				 hl=C[HLU]
	mlt	de		;		ude=L*BCU
	mlt	hl		;				uhl=HLU*C
	add	hl, de		;				uhl=HLU*C+L*BCU
	ld	d, b		;		 d =B
	ld	e, iyh		;		 de=BH
	mlt	de		;		ude=H*B
	add	hl, de		;				uhl=HLU*C+H*B+L*BCU
	add	a, h		; a=(HLU*C+H*B+L*BCU>>8)+E*C+HLU*B+H*BCU+L*A
	ld	h, l		;				 h =HLU*C+H*B+L*BCU
	ld	l, 0		;				 hl=HLU*C+H*B+L*BCU<<8
	ld	d, b		;		 d =B
	ld	e, iyl		;		 de=BL
	mlt	de		;		ude=L*B
	add.s	hl, de		;				uhl=L*B+(HLU*C+H*B+L*BCU<<8)&0xFFFF
	adc	a, 0		; a=L*B+(HLU*C+H*B+L*BCU<<8)+(E*C+HLU*B+H*BCU+L*A<<16)>>16
	ld	d, iyh		;		 d =H
	ld	e, c		;		 de=HC
	mlt	de		;		ude=H*C
	add	hl, de		;				uhl=H*C+(L*B+(HLU*C+H*B+L*BCU<<8)&0xFFFF)
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl		;				uhl=(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)
	adc	a, l		; a=H*C+L*B+(HLU*C+H*B+L*BCU<<8)+(E*C+HLU*B+H*BCU+L*A<<16)>>16
	ld	d, iyl		;		 d =L
	ld	e, c		;		 de=LC
	mlt	de		;		ude=L*C
	add	hl, de		;				uhl=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)
	adc	a, 0		; a=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)+(E*C+HLU*B+H*BCU+L*A<<24)>>24
	pop	de		;		ude=UDE							--
	ld	e, a		;		  e=H*C+L*B+(HLU*C+H*B+L*BCU<<8)+(E*C+HLU*B+H*BCU+L*A<<16)>>16
	pop	af		; a=A								--
	ret

; (EUHL*AUBC)&0xFFFFFFFF=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)+(E*C+HLU*B+H*BCU+L*A<<24)+(E*B+HLU*BCU+H*A<<32)+(E*BCU+HLU*A<<40)+(E*A<<48)&0xFFFFFFFF
;			=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)+(E*C+HLU*B+H*BCU+L*A<<24)&0xFFFFFFFF
