
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	ez80_utils_control
	XREF	_system_ticks

ez80_utils_control:
	POP	BC					; RESTORE BC AND HL
	POP	HL

	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, ez80_firmware_query			; B = 0
	DEC	A
	JR	Z, ez80_reg_ehl_to_hl			; B = 1
	DEC	A
	JR	Z, ez80_reg_hl_to_ehl			; B = 2

	LD	A, %FF					; UNKNOWN FUNCTION
	RET.L
;
; Function B = 00 -- ez80 Firmware Query
; Input
;   MBASE:HL{15:0} = Pointer to the configuration table
; Output
;   MBASE:HL{15:0} = Pointer to the ez80 firmware configuration table
;
; Input Table:
;  00-03: HBIOS Version?
;  04-07: CPU Build Frequency
;  08-15: RESERVED
;
; Output Table:
;  00-03: Firmware version (Major, Minor, Patch)??
;  04-07: CPU Build Frequency
;  08-11: CPU Evaluated Frequency
;  12-12: CHIP CODE (0->EZ80F92AZ020EG)
;  13-15: RESERVED
ez80_firmware_query:
	LD	HL, %000001 ; 0.0.0
	RET.L
;
; Function B = 01 -- register copy (8:16 -> 24)
; LD HL, E:HL{15:0}
; Input
;   E:HL{15:0} = value to be copied
; Output
;   HL{23:0} -> E:HL{15:0}
;
; not re-entrant safe
;
ez80_reg_ehl_to_hl:
	LD	(tmp), HL				; ONLY THE LOWER 16 BITS OF HL ARE VALID
	LD	A, E
	LD	(tmp+2), A				; SAVE THE UPPER 8 BITS OF THE 24 BIT VALUE
	LD	HL, (tmp)
	RET.L

; Function B = 02 -- register copy (24 -> 8:16)
; LD E:HL{15:0}, HL
; Input
;   HL{23:0} = value to be copied
; Output
;   E:HL{15:0} -> HL{23:0}
;
; not re-entrant safe
;
ez80_reg_hl_to_ehl:
	LD	(tmp), HL				; STORE THE FULL 24 BITS OF HL
	LD	A, (tmp+2)				; RETRIEVE THE UPPER 8 BITS
	LD	E, A
	RET.L

	SECTION BSS
tmp:
	DS	3

