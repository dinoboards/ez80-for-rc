
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	ez80_utils_control
	XREF	_system_ticks
	XREF	_platform_description
	XREF	_SYS_CLK_FREQ
	XREF	_cpu_freq_calculated
	XREF	__ldivu

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
;  00-03: CB_VERSION: 		HBIOS Version (RMJ, RMN, RUP, RTP)
;  04-07: CPUOSC: 		CPU Build Frequency
;  08-08: CB_PLATFORM: 		Platform ID
;  09-09: MEMMGR: 		Memory manager id
;  10-11: RAMSIZE:		Total size in KB
;  12-13: ROMSIZE:		Total size in KB
;  14-15: RESERVED
;
; Output Table:
;  00-03: EZ80_PLT_EZ80VER: 	Firmware version (Major, Minor, Patch)??
;  04-07: EZ80_PLT_CPUOSC: 	CPU Evaluated Frequency
;  08-08: EZ80_PLT_CPUMHZ: 	(CPU Evaluated Frequency / 1000000)
;  09-10: EZ80_PLT_CPUKHZ: 	(CPU Evaluated Frequency / 1000)
;  11-11: EZ80_PLT_CHIP_ID: 	(0->EZ80F92AZ020EG)
;  12-15: RESERVED
;
ez80_firmware_query:
	LD	B, 16
	LD	DE, _platform_description
	PUSH	HL
copy_platform_tbl:
	LD.S	A, (HL)
	LD	(DE), A
	INC	HL
	INC	DE
	DJNZ	copy_platform_tbl

	POP	IX
	PUSH	IX

	LD.S	(IX+0), 0				; ASSIGN VERSION NUMBER 0.0.0.1
	LD.S	(IX+1), 0
	LD.S	(IX+2), 0
	LD.S	(IX+3), 1

	LD	A, (_cpu_freq_calculated)
	LD.S	(IX+4), A
	LD	A, (_cpu_freq_calculated+1)
	LD.S	(IX+5), A
	LD	A, (_cpu_freq_calculated+2)
	LD.S	(IX+6), A
	LD	A, (_cpu_freq_calculated+3)
	LD.S	(IX+7), A

	LD	HL, (_cpu_freq_calculated)
	LD	E, A
	LD	BC, 1000000
	XOR	A
	CALL	__ldivu
	LD.S	(IX+8), L

	LD	HL, (_cpu_freq_calculated)
	LD	A, (_cpu_freq_calculated+3)
	LD	E,A
	LD	BC, 1000
	XOR	A
	CALL	__ldivu
	LD.S	(IX+9), L
	LD.S	(IX+10), H

	XOR	A					; ZERO OUT THE REMAINING BYTES
	LD.S	(IX+11), A
	LD.S	(IX+12), A
	LD.S	(IX+13), A
	LD.S	(IX+14), A
	LD.S	(IX+15), A

	POP	HL					; RESTORE TABLE ADDR
	RET.L

_system_build_freq:
	DL	_SYS_CLK_FREQ
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

