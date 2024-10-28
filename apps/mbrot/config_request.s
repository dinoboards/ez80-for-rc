	section	.text,"ax",@progbits
	.assume	adl = 1
	section	.text._getVideoMode,"ax",@progbits
	.global	_getVideoMode
_getVideoMode:
	ld	hl, _str
BB0_1:
	push	hl
	call	_puts
	pop	hl
	ld	hl, 1
	push	hl
	ld	hl, 4
	push	hl
	ld	hl, _input
	push	hl
	call	_fgets
	pop	hl
	pop	hl
	pop	hl
	ld	hl, _str.5
	push	hl
	call	_puts
	pop	hl
	ld	a, (_input)
	cp	a, 78
	jr	z, BB0_6
	cp	a, 80
	ld	hl, _str
	jr	z, BB0_5
	cp	a, 110
	jr	z, BB0_6
	cp	a, 112
	jr	nz, BB0_1
BB0_5:
	ld	a, 1
	ret
BB0_6:
	ld	a, 2
	ret
	section	.text._getVideoMode,"ax",@progbits

	section	.text._getLineCount,"ax",@progbits
	.global	_getLineCount
_getLineCount:
	ld	hl, _str.4
BB1_1:
	push	hl
	call	_puts
	pop	hl
	ld	hl, 1
	push	hl
	ld	hl, 4
	push	hl
	ld	hl, _input
	push	hl
	call	_fgets
	pop	hl
	pop	hl
	pop	hl
	ld	hl, _str.5
	push	hl
	call	_puts
	pop	hl
	ld	a, (_input)
	cp	a, 49
	jr	z, BB1_4
	cp	a, 50
	ld	hl, _str.4
	jr	nz, BB1_1
	ld	a, -44
	ret
BB1_4:
	ld	a, -64
	ret
	section	.text._getLineCount,"ax",@progbits

	section	.bss._input,"aw",@nobits
	.global	_input
_input:
	ds	4

	section	.rodata._str,"a",@progbits
	.local	_str
_str:
	db	"NTSC (n) or PAL (p) mode?", 13, 0

	section	.rodata._str.4,"a",@progbits
	.local	_str.4
_str.4:
	db	"Vertical dot count 212(2) or 192?", 13, 0

	section	.rodata._str.5,"a",@progbits
	.local	_str.5
_str.5:
	db	13, 0

	;ident	"clang version 15.0.0 (git@github.com:dinoboards/llvm-project.git 11bc9fe1fa8553b4b375141b9d4d32be9025a4b9)"
	.extern	__Unwind_SjLj_Register
	.extern	__Unwind_SjLj_Unregister
	.extern	_fgets
	.extern	_puts
