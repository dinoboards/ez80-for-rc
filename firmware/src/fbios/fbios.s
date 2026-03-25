	include "..\config.inc"
	include "..\rst-28-constants.inc"
	include "fbios-vars.inc"

	section	CODE
	.assume	adl=1

	xref	firmware_rst_28_hook
	xref	_print_string
	XREF	fbios_vars


; TODO - update main boot sequence
; 1. detect linear memory present and size (report it)
; 2. detect if RomWBW or MSX BIOS is present -eg if the legacy 64K memory mapped address is present
; 3. if detected, do the z80_invoke as current
; 4. otherwise jump to CP/M's BOOT handler here
; 4.1. identify the 64K chunk to use - choose highest available ram slot

; 5 - temp -- load a binary image that just print "STOP" and stops


; TODO Load from disk image, the CCP and BDOS - but not the BIOS
;   read from sector 4 for 11 sectors (512 byte sectors)
;   write to D000 up to (not including) E600


	PUBLIC	_fbios_hook
_fbios_hook:
	nop
	nop
	nop
	nop
	ret



; CONIN (function 3)
;
; Wait until the keyboard is ready to provide a character, and return it in A.
; TODO respect IOBYTE (supplied via A)
	PUBLIC	FB_CONIN
FB_CONIN:
	CALL	_fbios_hook

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
	CALL	_fbios_hook

	LD	E, C					; E = CHARACTER TO SEND
	LD	BC, +(BF_CIOOUT << 8) | CIO_CONSOLE
	JP	firmware_rst_28_hook

	PUBLIC	FB_LIST
FB_LIST:
	CALL	_fbios_hook

	LD	HL, __LIST
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__LIST:	DB	"LIST\r\n", 0


	PUBLIC	FB_PUNCH
FB_PUNCH:
	CALL	_fbios_hook

	LD	HL, __PUNCH
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__PUNCH:	DB	"PUNCH\r\n", 0

	PUBLIC	FB_READER
FB_READER:
	CALL	_fbios_hook

	LD	HL, __READER
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__READER:	DB	"READER\r\n", 0

	PUBLIC	FB_HOME
FB_HOME:
	CALL	_fbios_hook

	LD	IY, (fbios_vars)
	XOR	A
	SBC	HL,HL
	LD	(IY+FB_TRCK), HL
	RET.L

	PUBLIC	FB_LISTST
FB_LISTST:
	CALL	_fbios_hook

	LD	HL, __LISTST
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__LISTST:	DB	"LISTST\r\n", 0

	PUBLIC	FB_SECTRN
FB_SECTRN:
	CALL	_fbios_hook

	LD	HL, BC
	LD	HL, __SECTRN
	PUSH	HL
	CALL	_print_string
	POP	HL
	ret.L

__SECTRN:	DB	"SECTRN\r\n", 0


