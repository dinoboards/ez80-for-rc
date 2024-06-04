
	SECTION CODE

	.assume adl=1

	public	_jumpToRomWBWStart
_jumpToRomWBWStart:
	LD	A, 0B9h
	LD	MB, A
	JP.SIS	0

	extern	_uart_preinit
	public	uart_preinit
uart_preinit:
	CALL	_uart_preinit
	RET.L

	extern	_uart_init
	public	uart_init
uart_init:
	CALL	_uart_init
	RET.L

	public	_spike
_spike:

	LD	HL, step1
	ld	de, 0b7E000h
	ld	bc, step1_size
	ldir

	STMIX

	LD			A, 0B7H
	LD			MB, A
	LD.SIS		SP, 0E700H
	NOP
	NOP
	CALL.IS		0E000H
	RET


	PUBLIC	__special
__special:
	PUSH	IX
	LD	IX, 3
	ADD	IX, SP

	PUSH	HL
	PUSH	DE
	PUSH	BC
	PUSH	AF

	;LD	A, (IX+0) ; SHOULD BE 2H FOR Z80 MODE
	LD	L, (IX+1)
	LD	H, (IX+2) ; HL SHOULD BE RETURN ADDRESS - ADDRESS OF NEXT INSTRUCTION??

	LD.S 	A, (HL)  ; IS THIS THE FIRST BYTE OF THE NEXT INSTRUCTION?
	CP 	0D3H		; OUT (N), A
	JR	NZ, _unknown

	INC 	HL

	DB 052H, 04EH

	;ld.s	a, (HL)	; why cant we assemble ld.s c, (hl)
	;ld	c, a
	pop	af
	push	af

	LD B, 0FFH
	OUT (C), A

	inc	hl
	ld (ix+1), l
	ld (ix+2), h



_unknown:
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	POP IX

	NOP
	RET.L

	.assume adl = 0
step1:
	NOP
	LD	A, 15
	RST.L		8
	OUT (0), A
	NOP
	RET.L
step1_end:

step1_size	equ	step1_end-step1
