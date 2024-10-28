	section	.text,"ax",@progbits
	.assume	adl = 1
	section	.text._main,"ax",@progbits
	.global	_main
_main:
	ld	hl, -36
	call	__frameset
	call	_getVideoMode
	ld	l, a
	ld	(ix - 6), hl
	call	_getLineCount
	ld	l, a
	ld	(ix - 3), hl
	ld	de, (ix - 6)
	push	de
	push	hl
	call	_setMode6
	pop	hl
	pop	hl
	ld	hl, _palette
	push	hl
	call	_setPalette
	pop	hl
	or	a, a
	sbc	hl, hl
	push	hl
	call	_clearScreenBank0
	pop	hl
	ld	hl, _str
	push	hl
	call	_puts
	ld	iy, 0
	pop	hl
	lea	bc, iy + 0
	ld	hl, (ix - 3)
	ld	c, l
	xor	a, a
	call	__ultof
	push	bc
	pop	hl
	ld	e, a
	ld	bc, -8388608
	ld	a, 64
	call	__fdiv
	ld	hl, (ix - 3)
	ld	h, 0
	ld	(ix - 3), hl
	ld	(ix - 6), bc
	ld	(ix - 7), a
	lea	hl, iy + 0
	ld	e, 63
	call	__fmul
	ld	hl, _iY
	ld	(ix - 10), bc
	ld	bc, (ix - 3)
	ld	(ix - 11), a
	ld.sis	de, 0
BB0_1:
	ld	(hl), e
	inc	hl
	ld	(hl), d
	ld	l, e
	ld	h, d
	or	a, a
	sbc.sis	hl, bc
	jp	nc, BB0_17
	lea	bc, iy + 0
	ld	c, e
	ld	b, d
	xor	a, a
	call	__ultof
	ld	hl, (ix - 6)
	ld	e, (ix - 7)
	call	__fmul
	lea	hl, iy + 0
	ld	e, -64
	call	__fadd
	ld	l, a
	ld	(ix - 14), hl
	push	hl
	ld	(ix - 17), bc
	push	bc
	call	_fabsf
	pop	bc
	pop	bc
	ld	bc, (ix - 10)
	ld	a, (ix - 11)
	call	__fcmp
	ld	a, 1
	jp	m, BB0_4
	ld	a, 0
BB0_4:
	bit	0, a
	ld	iy, 0
	lea	hl, iy + 0
	jr	nz, BB0_6
	ld	hl, (ix - 17)
BB0_6:
	bit	0, a
	ld	a, 0
	jr	nz, BB0_8
	ld	de, (ix - 14)
	ld	a, e
BB0_8:
	ld	(_Cy), hl
	ld	(_Cy+3), a
	ld.sis	hl, 0
	ld	e, l
	ld	d, h
BB0_9:
	ld	hl, _iX
	ld	(hl), e
	inc	hl
	ld	(hl), d
	ld	l, e
	ld	h, d
	ld.sis	bc, 512
	or	a, a
	sbc.sis	hl, bc
	jp	nc, BB0_16
	lea	bc, iy + 0
	ld	c, e
	ld	b, d
	xor	a, a
	call	__ultof
	lea	hl, iy + 0
	ld	e, 60
	call	__fmul
	ld	hl, 2097152
	ld	e, -64
	call	__fadd
	ld	(ix - 31), bc
	ld	(_Cx), bc
	ld	(ix - 32), a
	ld	(_Cx+3), a
	ld	(_Zx), iy
	ld	a, l
	ld	(_Zx+3), a
	ld	(_Zy), iy
	ld	(_Zy+3), a
	ld	(_Zx2), iy
	ld	(_Zx2+3), a
	ld	(_Zy2), iy
	ld	(_Zy2+3), a
	ld	de, (_Cy)
	ld	(ix - 35), de
	ld	a, (_Cy+3)
	ld	(ix - 36), a
	ld	(ix - 20), iy
	ld	(ix - 21), l
	ld	(ix - 27), iy
	ld	(ix - 28), l
	lea	bc, iy + 0
	push	bc
	pop	de
	ex	de, hl
	ld	iyh, e
	ex	de, hl
	ld	(ix - 14), l
	ex	de, hl
	ld	iyl, e
	ex	de, hl
	ex	de, hl
BB0_11:
	ld	(ix - 17), bc
	ld	a, (ix - 14)
	ld	(ix - 24), hl
	ld	e, iyh
	call	__fadd
	ld	e, a
	ld	a, iyl
	cp	a, 16
	jp	nc, BB0_14
	push	bc
	pop	hl
	ld	bc, -8388608
	ld	a, 64
	call	__fcmp
	jp	p, BB0_14
	ld	bc, (ix - 27)
	ld	a, (ix - 28)
	or	a, a
	sbc	hl, hl
	ld	e, 64
	call	__fmul
	ld	hl, (ix - 20)
	ld	e, (ix - 21)
	call	__fmul
	ld	hl, (ix - 35)
	ld	e, (ix - 36)
	call	__fadd
	ld	(ix - 20), bc
	ld	d, a
	ld	(_Zy), bc
	ld	(_Zy+3), a
	ld	bc, (ix - 17)
	ld	a, (ix - 14)
	ld	hl, (ix - 24)
	ld	e, iyh
	call	__fsub
	push	bc
	pop	hl
	ld	e, a
	ld	bc, (ix - 31)
	ld	a, (ix - 32)
	call	__fadd
	ld	(_Zx), bc
	ld	(_Zx+3), a
	ld	(ix - 27), bc
	push	bc
	pop	hl
	ld	(ix - 28), a
	ld	e, a
	call	__fmul
	ld	(ix - 17), bc
	ld	(ix - 14), a
	ld	(_Zx2), bc
	ld	(_Zx2+3), a
	ld	bc, (ix - 20)
	ld	a, d
	push	bc
	pop	hl
	ld	(ix - 21), d
	ld	e, d
	call	__fmul
	push	bc
	pop	hl
	ld	bc, (ix - 17)
	ld	iyh, a
	ld	(_Zy2), hl
	ld	(_Zy2+3), a
	inc	iyl
	jp	BB0_11
BB0_14:
	ld	a, iyl
	ld	(_iteration), a
	call	_cRawIo
	or	a, a
	ld	iy, 0
	jr	nz, BB0_17
	ld	hl, _iX
	ld	hl, (hl)
	ld	iy, __fromX
	ld	(iy), l
	ld	(iy + 1), h
	ld	hl, _iY
	ld	hl, (hl)
	ld	iy, __fromY
	ld	(iy), l
	ld	(iy + 1), h
	ld	a, (_iteration)
	ld	(__color), a
	ld	a, 80
	ld	(__operation), a
	call	_commandDrawLine
	ld	iy, 0
	ld	hl, _iX
	ld	de, (hl)
	inc.sis	de
	jp	BB0_9
BB0_16:
	ld	hl, _iY
	ld	de, (hl)
	inc.sis	de
	ld	bc, (ix - 3)
	jp	BB0_1
BB0_17:
	lea	hl, iy + 0
	ld	sp, ix
	pop	ix
	ret
	section	.text._main,"ax",@progbits

	section	.data._palette,"aw",@progbits
	.global	_palette
_palette:
	ds	3
	db	1
	db	0
	db	0
	db	2
	db	0
	db	0
	db	3
	db	0
	db	0
	db	4
	db	0
	db	0
	db	5
	db	0
	db	0
	db	6
	db	0
	db	0
	db	7
	db	0
	db	0
	db	8
	db	0
	db	0
	db	9
	db	0
	db	0
	db	10
	db	0
	db	0
	db	11
	db	0
	db	0
	db	12
	db	0
	db	0
	db	13
	db	0
	db	0
	db	14
	db	0
	db	0
	db	15
	db	0
	db	0

	section	.bss._iY,"aw",@nobits
	.global	_iY
_iY:
	ds	2

	section	.bss._Cy,"aw",@nobits
	.global	_Cy
_Cy:
	ds	4

	section	.bss._iX,"aw",@nobits
	.global	_iX
_iX:
	ds	2

	section	.bss._Cx,"aw",@nobits
	.global	_Cx
_Cx:
	ds	4

	section	.bss._Zx,"aw",@nobits
	.global	_Zx
_Zx:
	ds	4

	section	.bss._Zy,"aw",@nobits
	.global	_Zy
_Zy:
	ds	4

	section	.bss._Zx2,"aw",@nobits
	.global	_Zx2
_Zx2:
	ds	4

	section	.bss._Zy2,"aw",@nobits
	.global	_Zy2
_Zy2:
	ds	4

	section	.bss._iteration,"aw",@nobits
	.global	_iteration
_iteration:
	ds	1

	section	.rodata._str,"a",@progbits
	.local	_str
_str:
	db	"Press any key to abort", 13, 0

	;ident	"clang version 15.0.0 (git@github.com:dinoboards/llvm-project.git 11bc9fe1fa8553b4b375141b9d4d32be9025a4b9)"
	.extern	_fabsf
	.extern	_cRawIo
	.extern	_setMode6
	.extern	__Unwind_SjLj_Register
	.extern	_commandDrawLine
	.extern	__frameset
	.extern	_setPalette
	.extern	__fadd
	.extern	__operation
	.extern	__ultof
	.extern	_puts
	.extern	__fmul
	.extern	__color
	.extern	__fromX
	.extern	__fromY
	.extern	__Unwind_SjLj_Unregister
	.extern	_getVideoMode
	.extern	__fdiv
	.extern	_getLineCount
	.extern	_clearScreenBank0
	.extern	__fcmp
	.extern	__fsub
