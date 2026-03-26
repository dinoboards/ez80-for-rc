	include "..\config.inc"
	include "..\rst-28-constants.inc"
	include "fbios-vars.inc"

	section	CODE
	.assume	adl=1

	xref	firmware_rst_28_hook
	xref	_print_string
	XREF	fbios_vars


; CONIN (function 3)
;
; Wait until the keyboard is ready to provide a character, and return it in A.
; TODO respect IOBYTE (supplied via A)
	PUBLIC	FB_CONIN
FB_CONIN:
	LD	B, BF_CIOIN
	LD	C, CIO_CONSOLE
	CALL.IL firmware_rst_28_hook

	LD	A, E					; MOVE CHARACTER RETURNED TO A
	RET.L

; CONOUT (function 4)
;
; Write the character in C to the screen.
; TODO respect IOBYTE (supplied via A)
	PUBLIC	FB_CONOUT
FB_CONOUT:
	LD	E, C					; E = CHARACTER TO SEND
	LD	BC, +(BF_CIOOUT << 8) | CIO_CONSOLE
	JP	firmware_rst_28_hook

	PUBLIC	FB_LIST
FB_LIST:
	LD	HL, __LIST
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__LIST:	DB	"LIST\r\n", 0


	PUBLIC	FB_PUNCH
FB_PUNCH:
	LD	HL, __PUNCH
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__PUNCH:	DB	"PUNCH\r\n", 0

	PUBLIC	FB_READER
FB_READER:
	LD	HL, __READER
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__READER:	DB	"READER\r\n", 0

	PUBLIC	FB_HOME
FB_HOME:
	LD	IY, (fbios_vars)
	XOR	A
	SBC	HL,HL
	LD	(IY+FB_TRCK), HL
	RET.L

	PUBLIC	FB_LISTST
FB_LISTST:
	LD	HL, __LISTST
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__LISTST:	DB	"LISTST\r\n", 0

	PUBLIC	FB_SECTRN
FB_SECTRN:
	LD	HL, BC
	LD	HL, __SECTRN
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__SECTRN:	DB	"SECTRN\r\n", 0


