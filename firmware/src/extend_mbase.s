	INCLUDE "config.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

	; uint24_t extend_mbase(uint16_t address)

;---------------------------------------------------------
; Assign the upper byte of HL to MBASE
;
; UHL = <MBASE>HL
;
; Input:      HL = a 16 bit address within the current
;                  segment
; Output:     HL = the full 24 bit address
;
;---------------------------------------------------------

	PUBLIC	_extend_mbase
_extend_mbase:
	LD	IY, 0
	ADD	IY, SP
	LD	A, MB
	LD	(IY+5),A
	LD	HL, (IY+3)
	RET
