	include "config.inc"
	include "rst-28-vars.inc"

	SECTION CODE
	.ASSUME ADL=1

	PUBLIC	_putchar
	PUBLIC	_putch
	XREF	_hbios_vars
	XREF	uart_out
	XREF	cioout

;---------------------------------------------------------
; Implement standard clib putch(int) and putchar(int)
;
; Will initially send supplied char to serial (UART0).
; But once, and if, hbios has been initiated, then
; send to the default (index 0) driver
;
; Output:     HL  = 0
;---------------------------------------------------------

_putch:
_putchar:
	LD	IY, 0
	ADD	IY, SP
	LD	E, (IY+3)				; save char to be sent in E

	LD	A, (_hbios_vars+2)			; has hbios_vars been assigned a 64K segment?
	OR	A
	JR	Z, L_1					; no, send to uart0
	LD	IY, (_hbios_vars)
	LD	A, (IY+HB_CIOCNT)			; has the default hbios cio drivers been installed?
	OR	A
	JR	Z, L_1					; no, send to uart0

	LD	C, 0
	CALL.IL	cioout					; send to cio device (crt, serial, or other)
	JR	L_2

L_1:
	CALL.IL	uart_out

L_2:
	XOR	A					; return 0
	SBC	HL, HL
	RET
