
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
	LD	A, 3			; UART
	LD	B, 5			; UART-QUERY
	RST.L	%10			;


	LD	A, 3			; UART
	LD	B, 4			; UART-CONFIG
	LD	HL, 9600		; desired baud rate
	; LD	DE, 3 			; no parity, 1 stop bit
	; LD	DE, 3 | 4		; TODO: set parity to even, 2 STOP BITS
	; LD	DE, 2 << 3		; 7 STOP BITS
	; LD	DE, 3 << 3		; 8 STOP BITS
	LD	DE, 3 << 3 | %20	; 8 STOP BITS | CTS FLOW CONTROL
	RST.L	%10			;


	LD 	HL, 0
	LD 	DE, 0
	LD	A, 3			; UART
	LD	B, 5			; UART-QUERY
	RST.L	%10			;

	NOP
	NOP

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

; 	LD	B, 0
; WAIT:
; 	PUSH	BC
; 	POP	BC
; 	DJNZ	WAIT

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

	LD.LIL	HL, 115200

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
