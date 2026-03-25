
	INCLUDE "..\config.inc"
	INCLUDE "fbios-vars.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

	XREF	fbios_vars

;---------------------------------------------------------
; Implement CBIOS SETSEC BIOS Function (function 11)
;
; Set the sector number for the next disk operation.
;
; Input:      BC = sector number (16-bit).
; Output:     None.
;---------------------------------------------------------

	PUBLIC	FB_SETSEC
FB_SETSEC:
	LD	IY, (fbios_vars)
	LD	(IY+FB_SEC+0), C
	LD	(IY+FB_SEC+1), B
	XOR	A
	LD	(IY+FB_SEC+2), A
	RET.L
