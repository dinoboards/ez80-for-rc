	INCLUDE "..\config.inc"
	INCLUDE "..\rst-28-constants.inc"
	INCLUDE "fbios-vars.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

;---------------------------------------------------------
; Implement CBIOS READ BIOS Function (function 13)
;
; Read the currently set track and sector at the current DMA address.
;
; Delegates to the fbios_read function
;
; Output:     A  = 0 for OK, 1 for unrecoverable error, 0FFh if media changed.
;---------------------------------------------------------
	XREF	_fbios_read
	PUBLIC	FB_READ
FB_READ:
	CALL	_fbios_read
	OR	A
	RET.L
