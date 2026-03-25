
	INCLUDE "..\config.inc"
	INCLUDE "fbios-vars.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

	XREF	fbios_vars

;---------------------------------------------------------
; Implement CBIOS SETTRK BIOS Function (function 10)
;
; Set the track number for the next disk operation.
;
; Input:      BC = track number (16-bit, 0-based).
; Output:     None.
;---------------------------------------------------------
	PUBLIC	FB_SETTRK
FB_SETTRK:
	LD	IY, (fbios_vars)
	LD	(IY+FB_TRCK+0), C
	LD	(IY+FB_TRCK+1), B
	XOR	A
	LD	(IY+FB_TRCK+2), A
	RET.L
