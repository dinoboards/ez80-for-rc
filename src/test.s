
        INCLUDE "startup\ez80f92.inc"

	SECTION CODE

	.assume adl=1

; UART0_LSR	.EQU	$C5
; UART0_THR	.EQU	$C0
; UART0_RBR	.EQU	$C0



	public	_spike
	XREF	_rx_buffer_empty
	XREF	_rx_buffer_get
	ei

_spike:
	ld	e, '>'
	LD	A, 3			; UART
	LD	B, 1			; UART-OUT
	RST.L	%10

	ld	e, ' '
	LD	A, 3			; UART
	LD	B, 1			; UART-OUT
	RST.L	%10

EZUART_IN:
	LD	A, 3			; UART
	LD	B, 0			; UART-IN
	RST.L	%10			; CHAR RETURNED IN E

	push	de
	LD	A, 3			; UART
	LD	B, 2			; UART-IST
	RST.L	%10			; BUF LENGTH IN A
	pop	de

EZUART_OUT:
	LD	A, 3			; UART
	LD	B, 1			; UART-OUT
	RST.L	%10

	JR	EZUART_IN


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
