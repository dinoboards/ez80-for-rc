
        INCLUDE "startup\ez80f92.inc"

	SECTION CODE

	.assume adl=1

	public	_spike

	XREF	__INTERNAL_RAM_RAM
	XREF	__INTERNAL_RAM_LEN
_spike:
	ei

	; write spike code here to be run in ADL mode on  on-chip rom

RAM_FREE	.EQU	__INTERNAL_RAM_RAM+__INTERNAL_RAM_LEN
RAM_FREE_BASE	.EQU	RAM_FREE & %FFFF

	; copy code from rom to ram and run it in Z80 mode
	LD	HL, step1
	ld	de, RAM_FREE
	ld	bc, step1_size
	ldir

	LD	A, 02H
	LD	MB, A
	LD.SIS	SP, 0EA00H
	NOP
	NOP
	CALL.IS	RAM_FREE_BASE		; run the z80 code in RAM
	RET


	SEGMENT CODE

	.assume adl = 0
step1:

	NOP
	JR	step1
	; write code here to be run in z80 mode on on-chip rom

step1_end:

step1_size	equ	step1_end-step1
