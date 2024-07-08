
        INCLUDE "startup\ez80f92.inc"

	SECTION CODE

	.assume adl=1

	public	_spike
_spike:
	LD	HL, step1
	ld	de, 0b7E000h
	ld	bc, step1_size
	ldir

	LD	A, 0B7H
	LD	MB, A
	LD.SIS	SP, 0E700H
	NOP
	NOP
	CALL.IS	0E000H
	RET

	.assume adl = 0
step1:
	ei
loop:
	nop
	nop
	jr loop

	NOP

	RET.L
step1_end:

step1_size	equ	step1_end-step1
