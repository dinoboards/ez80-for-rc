	include "../config.inc"
	include "../rst-28-constants.inc"

	SECTION CODE

	.ASSUME ADL=1

	XREF	firmware_rst_10_hook
	XREF	_baud_rate
	XREF	_line_control

DISP_UART	EQU	3

	PUBLIC	_uart0_cioin
	XREF	uart_in
_uart0_cioin:	EQU	uart_in


	PUBLIC	_uart0_cioout
	XREF	uart_out
_uart0_cioout:	EQU	uart_out

	PUBLIC	_uart0_cioist
	XREF	uart_ist
_uart0_cioist:	EQU	uart_ist

	PUBLIC	_uart0_cioost
	XREF	uart_ost
_uart0_cioost:	EQU	uart_ost

	PUBLIC	_uart0_ciodevice
_uart0_ciodevice:
	LD	C, 0
	LD	D, CIODEV_EZ80UART
	LD	E, 0
	LD	HL, 0
	RET.L

	PUBLIC	_uart0_cioquery
_uart0_cioquery:
	LD	A, (_line_control)
	LD	B, A

	XOR	A
	OR	3 << 3		; ISOLATE DATA BITS
	AND	B		; MASK IN DATA BITS

	RRCA			; SHIFT TO BITS 1:0
	RRCA
	RRCA
	LD	H, A		; H{1:0} DATA BITS

	BIT	2, B		; STOP BITS
	JR	Z, SKIP1
	SET	2, H		; APPLY TO H

SKIP1:
	BIT	1, B		; PARITY ENABLE
	JR	Z, SKIP2
	SET	3, H		; APPLY TO H

SKIP2:
	BIT	0, B		; EVEN PARITY
	JR	Z, SKIP3
	SET	4, H		; APPLY TO H

SKIP3:
	LD	DE, 0
	LD	E, H
	LD	HL, (_baud_rate)
	RET.L
