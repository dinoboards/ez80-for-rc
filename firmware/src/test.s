
if 0
        INCLUDE "startup\ez80F92.inc"

	SECTION CODE

	.assume adl=1

	public	_spike

	XREF	z80_invoke
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

	; call	z80_invoke
	CALL.IS	%E000		; run the z80 code in RAM
	RET

	SEGMENT CODE

	.assume adl = 0

step1:
	NOP
	ld	a, %21
	ex	af, af'
	ld	a, %12

	ld	bc, %6543
	ld	de, %a987
	ld	hl, %edcb
	exx
	ld	bc, %3456
	ld	de, %789a
	ld	hl, %bcde

	ld	iy, %1122
	ld	ix, %2233

	db	%CB, %30		; switch to emulated mode

	nop		; executed by emulator
	exx		; executed by emulator
	ex	af, af'	; executed by emulator

	db	%cb, %31		; switch to native code

	nop
	exx
	ex	af, af'
	nop

	ret.l

	; write code here to be run in z80 mode on on-chip rom

step1_end:

step1_size	equ	step1_end-step1

endif
