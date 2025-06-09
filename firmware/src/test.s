
if 0
        INCLUDE "startup\ez80F92.inc"

	SECTION CODE

	.assume adl=1

	public	_spike

	XREF	z80_invoke_iy
	XREF	__INTERNAL_RAM_RAM
	XREF	__INTERNAL_RAM_LEN
_spike:
	di

	; write spike code here to be run in ADL mode on  on-chip rom

RAM_FREE	.EQU	__INTERNAL_RAM_RAM+__INTERNAL_RAM_LEN
RAM_FREE_BASE	.EQU	RAM_FREE & %FFFF

	; copy code from rom to ram and run it in Z80 mode
	LD	HL, step1
	ld	de, %02E000
	ld	bc, step1_size
	ldir

	LD	A, 02H
	LD	MB, A
	LD.SIS	SP, 0EA00H
	NOP
	NOP
	ld	iy, %02E000

	call	z80_invoke_iy
	;CALL.IS	%E000		; run the z80 code in RAM
	;RET

	SEGMENT CODE

	.assume adl = 0

step1:
	ld	hl, %0040
	ld	ix, %0000

	lea	hl, ix+0

	ld	a, l
	or	a
	jr	z, _detected_ez80
_z80:
	nop
	nop
	jr	_z80

_detected_ez80:
	ld	a, 0
	ld	b, 0
	ld	c, 2
	ld	d, 1
	ld	e, 0
	ld	hl, 0
	rst.l	%10	; exchange ver

	ld	a, 3
	ld	b, 1
	ld	e, 65
	rst.l	%10

	ld	a, 3
	ld	b, 1
	ld	e, 66
	rst.l	%10

	ld	a, 3
	ld	b, 1
	ld	e, 67
	rst.l	%10


loop:
	nop
	jr	loop

	; write code here to be run in z80 mode on on-chip rom

step1_end:

step1_size	equ	step1_end-step1

endif
