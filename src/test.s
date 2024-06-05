
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
	NOP
	LD	A, 15
	RST.L	%08		; I/O HELPER
	OUT 	(0), A


	;LD	A, 2
	;LD	BC, %1234
	;RST.L	%10		; ACCESS FUNCTION 2

	;LD	A, %03		; bank to switch to
	;RST.L	%18		; fast bank switchiing


	RET.L
step1_end:

step1_size	equ	step1_end-step1
