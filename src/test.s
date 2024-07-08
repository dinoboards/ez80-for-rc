
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

	LD	DE, 0
	LD	HL, 0
	LD	A, 2
	LD	B, 0	; GET TIMER TICK
	RST.L	%10

loop:
	LD	DE, 0
	LD	HL, 0
	LD	A, 2
	LD	B, 1	; GET SECS TICK
	RST.L	%10

	nop
	nop
	jr loop

	NOP

	RET.L
step1_end:

step1_size	equ	step1_end-step1
