
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	ez80_utils_control

ez80_utils_control:
	POP	BC		; restore bc and hl
	POP	HL

	LD	A, B		; SUB FUNCTION CODE
	OR	A		; TEST SUB FUNCTION CODE
	JR	Z, ez80_firmware_init	; B = 0
	DEC	A
	JR	Z, ez80_reg_ehl_to_hl	; B = 1

	LD	A, %FF		; UNKNOWN FUNCTION
	RET.L
;
; Function B = 00 -- Initialise the ez80 firmware
; Input
;   MBASE:HL{15:0} = Pointer to the ez80 firmware configuration table
; Output
;   HL -> version number
ez80_firmware_init:
	LD	HL, %000001 ; 0.0.0
	RET.L
;
; Function B = 01 -- register copy
; Input
;   E:HL{15:0} = value to be copied
; Output
;   HL{23:0} -> E:HL{15:0}
;
; not re-entrant safe
;
ez80_reg_ehl_to_hl:
	LD	(tmp), HL	; only the lower 16 bits of HL are valid
	ld	a, e
	ld	(tmp+2), A	; save the upper 8 bits of the 24 bit baud rate
	ld	hl, (tmp)
	RET.L

	SECTION BSS
tmp:
	DS	3
