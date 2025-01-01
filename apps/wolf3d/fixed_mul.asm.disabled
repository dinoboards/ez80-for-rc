	section	.text, "ax", @progbits
	.assume	adl=1
	.global	_FixedMul
	.extern __lmulu

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

   ; Multiplies EUHL by AUBC and returns the 32-bit product euhl.
	; jp	__lmulu


; Multiplies UHL by UBC and returns the 32-bit product euhl.
;    I: UBC=multiplier, UHL=multiplicand, ADL=1
;    O: a=EUHL*AUBC-L*C>>24, ubc=L*C, deu=0, d=H*C>>8, euhl=EUHL*AUBC
;   FO: sz(e), vc((EUHL*AUBC-L*C&0xFFFFFFFF)+L*C)
;   CC: (86+2*cf)*r(PC)+9*w(SPL)+9*r(SPL)+42
;				; a		ubc		ude		uhl		uiy		u(sp)
Mul_EUHL_AUBC_EUHL_Fast:	;--------------+---------------+---------------+---------------+---------------+------------------------
	xor	a

	; ld	d, c		;				 de=C0
	; e is 0, therefore de=0
	; mlt	de		;				ude=0*C
	; ld.sis de, 0
	; a is 0, therefore d is 0

	;ld	d, a		;				 d =0
	;ld	a, e		; a=E*C

	ld	e, l		; 				 de=0L
	ld	iyl, e		;								iyl=L

	;d is still 0, therefore de is 0
	; mlt	de		;				ude=L*0
	; ld.sis de, 0

	; therefore a is 0
	; add	a, e		; a=0*C+L*0

	dec	sp
	push	bc
	inc	sp
	inc	sp
	push	hl
	inc	sp
	inc	sp		;											 (sp)=[BCU][HLU]
	ld	d, h		;				 d =H
	ld	iyh, d		;								 iy=HL
	pop	hl		;						 hl=[BCU][HLU]				--
	ld	e, h		;				 de=H[BCU]
	mlt	de		;				ude=H*BCU
	add	a, e		; a=E*C+H*BCU+L*A
	ld	d, b		;				 d =B
	ld	e, l		;				 de=B[HLU]
	mlt	de		;				ude=HLU*B
	add	a, e		; a=E*C+HLU*B+H*BCU+L*A
	ld	d, h		;				 d =BCU
	ld	e, iyl		;				 de=[BCU]L
	ld	h, c		;						 hl=C[HLU]
	mlt	de		;				ude=L*BCU
	mlt	hl		;						uhl=HLU*C
	add	hl, de		;						uhl=HLU*C+L*BCU
	ld	d, b		;				 d =B
	ld	e, iyh		;				 de=BH
	mlt	de		;				ude=H*B
	add	hl, de		;						uhl=HLU*C+H*B+L*BCU
	add	a, h		; a=(HLU*C+H*B+L*BCU>>8)+E*C+HLU*B+H*BCU+L*A
	ld	h, l		;						 h =HLU*C+H*B+L*BCU
	ld	l, 0		;						 hl=HLU*C+H*B+L*BCU<<8
	ld	d, b		;				 d =B
	ld	e, iyl		;				 de=BL
	mlt	de		;				ude=L*B
	add.s	hl, de		;						uhl=L*B+(HLU*C+H*B+L*BCU<<8)&0xFFFF
	adc	a, 0		; a=L*B+(HLU*C+H*B+L*BCU<<8)+(E*C+HLU*B+H*BCU+L*A<<16)>>16
	ld	d, iyh		;				 d =H
	ld	e, c		;				 de=HC
	mlt	de		;				ude=H*C
	add	hl, de		;						uhl=H*C+(L*B+(HLU*C+H*B+L*BCU<<8)&0xFFFF)
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl		;						uhl=(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)
	adc	a, l		; a=H*C+L*B+(HLU*C+H*B+L*BCU<<8)+(E*C+HLU*B+H*BCU+L*A<<16)>>16
	ld	b, iyl		;		 bc =LC
	mlt	bc		;		ubc=L*C
	add	hl, bc		;						uhl=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)
	ld	e, a		;				  e=H*C+L*B+(HLU*C+H*B+L*BCU<<8)+(E*C+HLU*B+H*BCU+L*A<<16)>>16
	ret	nc		; nc => e=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)+(E*C+HLU*B+H*BCU+L*A<<24)>>24
	inc	e		; 				e=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)+(E*C+HLU*B+H*BCU+L*A<<24)>>24
	ret

; (EUHL*AUBC)&0xFFFFFFFF=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)+(E*C+HLU*B+H*BCU+L*A<<24)+(E*B+HLU*BCU+H*A<<32)+(E*BCU+HLU*A<<40)+(E*A<<48)&0xFFFFFFFF
;			=L*C+(H*C+L*B<<8)+(HLU*C+H*B+L*BCU<<16)+(E*C+HLU*B+H*BCU+L*A<<24)&0xFFFFFFFF
