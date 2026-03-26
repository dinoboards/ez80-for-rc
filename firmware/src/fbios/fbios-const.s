	include "..\config.inc"
	include "..\rst-28-constants.inc"

	SEGMENT	CODE
	.ASSUME ADL=1

	XREF	firmware_rst_28_hook

;---------------------------------------------------------
; Implement CBIOS CONST BIOS Function (function 2)
;
; Returns its status in A; 0 if no character is ready, 0FFh if one is.
; TODO respect IOBYTE (supplied via A)
;
; Output:     A  = 0 if no character ready, 0FFh if character ready
;---------------------------------------------------------

	PUBLIC	FB_CONST
FB_CONST:
	LD	B, BF_CIOIST
	LD	C, CIO_CONSOLE
	CALL.IL firmware_rst_28_hook

	OR	A					; SET FLAGS
	RET.L	Z					; NO CHARACTERS WAITING (IST) OR OUTPUT BUF FULL (OST)
	OR	%FF					; $FF SIGNALS READY TO READ (IST) OR WRITE (OST)
	RET.L
