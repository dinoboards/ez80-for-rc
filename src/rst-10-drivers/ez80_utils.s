
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	ez80_utils_control
	XREF	HB_TICKS

ez80_utils_control:
	POP	BC		; restore bc and hl
	POP	HL

	LD	A, B		; SUB FUNCTION CODE
	OR	A		; TEST SUB FUNCTION CODE
	JR	Z, ez80_firmware_query	; B = 0
	DEC	A
	JR	Z, ez80_reg_ehl_to_hl	; B = 1

	LD	A, %FF		; UNKNOWN FUNCTION
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


	PUBLIC	_measure_cpu_freq
	XREF	HB_TICKS
	XREF	_assign_cpu_frequency

_measure_cpu_freq:
	EI
	; // use timer ticks (60Hz) to evaluate the current CPU frequency
repeat:
	LD	HL, HB_TICKS

	LD	DE, 0
	ld	bc, 0

	LD	A, (HL)
sync_tick:
	CP	(HL)				; SYNC TO NEXT TICK EDGE
	JR	Z, sync_tick

	ld	a, (hl)

cycle_counter:
	INC	DE									; 1 CYCLE
	CP	(HL)				; SYNC TO NEXT TICK EDGE		; 2 CYCLES
	JR	Z, cycle_counter							; 3 CYCLES

skip:
	PUSH	DE
	call	_assign_cpu_frequency
	POP	DE

	DI
	RET

	SECTION BSS
tmp:
	DS	3

	PUBLIC	_cpu_freq_calculated
_cpu_freq_calculated:
	DS	4
