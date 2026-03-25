	INCLUDE "..\config.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

;---------------------------------------------------------
; Implement CBIOS WRITE BIOS Function (function 14)
;
; Write the currently set track and sector.
;
; Delegates to c function fbios_write
;
; Input:      C  = Deblocking code:
;                  0 = Write can be deferred
;                  1 = Write must be immediate
;                  2 = Write can be deferred, no pre-read is necessary.
; Output:     A  = 0 for OK,
;                  1 for unrecoverable error,
;                  2 if disc is readonly,
;                  0FFh if media changed.
;---------------------------------------------------------

	XREF	_fbios_write
	PUBLIC	FB_WRITE
FB_WRITE:
	PUSH	BC
	CALL	_fbios_write
	POP	BC
	OR	A
	RET.L
