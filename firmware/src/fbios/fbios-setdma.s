
	INCLUDE "..\config.inc"
	INCLUDE "fbios-vars.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

	XREF	fbios_vars

;---------------------------------------------------------
; Implement CBIOS SETDMA BIOS Function (function 12)
;
; The next disc operation will read its data from, or write its data to, the
; address given in BC.
;
; Input:      BC = DMA address.
; Output:     None.
;---------------------------------------------------------
	PUBLIC	FB_SETDMA
FB_SETDMA:
	LD	IY, (fbios_vars)
	LD	(IY+FB_DMA), BC
	RET.L
