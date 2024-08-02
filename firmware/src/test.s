
        INCLUDE "startup\ez80f92.inc"

	SECTION CODE

	.assume adl=1

	public	_spike

_spike:
	ei

	; write spike code here to be run in ADL mode on  on-chip rom


	; copy code from rom to ram and run it in Z80 mode
	LD	HL, step1
	ld	de, 0b7E000h
	ld	bc, step1_size
	ldir

	LD	A, 0B7H
	LD	MB, A
	LD.SIS	SP, 0E700H
	NOP
	NOP
	CALL.IS	0E000H		; run the z80 code in RAM
	RET


	SEGMENT CODE

	.assume adl = 0
step1:

	; write code here to be run in z80 mode on on-chip rom

	NOP

	RET.L
step1_end:

step1_size	equ	step1_end-step1
