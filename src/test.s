
        INCLUDE "startup\ez80f92.inc"

	SECTION CODE

	.assume adl=1

; UART0_LSR	.EQU	$C5
; UART0_THR	.EQU	$C0
; UART0_RBR	.EQU	$C0

LSR_THRE	.EQU	%20
LSR_DR		.EQU	%01

	public	_spike
	XREF	_rx_buffer_empty
	XREF	_rx_buffer_get
	ei

_spike:
	ld	e, '>'
	call	out_byte
	ld	e, ' '
	call	out_byte

EZUART_IN:
	CALL	_rx_buffer_empty
	OR	A
	JR	NZ, EZUART_IN

	CALL	_rx_buffer_get
	LD	A, E			; char should be in E

EZUART_OUT:
	CALL	out_byte

	JR	EZUART_IN

out_byte:
	; LD	A, 2			; UART
	; LD	B,

	IN0	A, (UART0_LSR)		; WAIT FOR TX READY
	AND	LSR_THRE
	JR	Z, EZUART_OUT

	OUT0	(UART0_THR), E		; SEND THE CHAR
	ret

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


	SEGMENT CODE


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
