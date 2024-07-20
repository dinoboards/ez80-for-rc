
        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	ez80_utils_control
	XREF	_system_ticks
	XREF	_platform_description
	XREF	_SYS_CLK_FREQ
	XREF	_cpu_freq_calculated
	XREF	__ldivu
	XREF	__stoiu
	XREF	__ladd_b
	XREF	_cs_bus_timings

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
	DEC	A
	JR	Z, ez80_set_bus_cycles			; B = 3
	DEC	A
	JR	Z, ez80_set_bus_freq			; B = 4

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

	SECTION	CODE

; Function B = 03 -- Set Bus cycles
; Input
;   H = CS3 External Memory Bus Cycles (1-15)
;   L = CS2 External I/O Bus Cycles (1-15)
;
; Output
;   A = 0 -> SUCCESS, NZ -> ONE ORE MORE VALUES OUT OF RANGE
;
ez80_set_bus_cycles:
	PUSH	IX
	LD	IX, _cs_bus_timings
	LD	(IX+0), L
	LD	(IX+1), H

	; Assign CS2 bus mode
	LD	A, L
	AND	%0F
	OR	BMX_BM_Z80 | BMX_AD_SEPERATE
	OUT0	(CS2_BMC), A

	; Assign CS3 bus mode
	LD	A, H
	AND	%0F
	OR	BMX_BM_Z80 | BMX_AD_SEPERATE
	OUT0	(CS3_BMC), A

	POP	IX
	XOR	A
	RET.L


; Function B = 03 -- Set Bus cycles to achive an equivalent frequency
; Input
;   HL = CS3 External Memory Bus Frequency (Khz)
;   DE = CS2 External I/O Bus Frequency (Khz)
;
; Output
;   H = CS3 External Memory Bus Cycles (1-15)
;   L = CS2 External I/O Bus Cycles (1-15)
;   A = 0 -> SUCCESS, NZ -> ONE ORE MORE VALUES OUT OF RANGE
;
ez80_set_bus_freq:
	; TO CALCULATE BUS CYCLES FOR A DESIRED BUS FREQUENCY WE USE:
	; CYCLES = (_cpu_freq_calculated / 1000) / target_bus_frequency_khz;
	; FACTOR THE VALUES TO ENABLE ROUNDING TO NEAREST WHOLE CYCLE:
	; (((_cpu_freq_calculated/100) / target_bus_frequency)+5)/10;

	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	PUSH	HL					; PERSIST DESIRED CS3 BUS FREQ (IX-3)
	PUSH	DE					; PERSIST DESIRED CS2 BUS FREQ (IX-6)
	PUSH	BC					; RESERVE 3 BYTES FOR TEMP STORAGE (IX-9)
	DEC	SP					; RESERVE 2 BYTE FOR TEMP STORAGE (IX-10)

	LD	HL, (_cpu_freq_calculated)
	LD	A, (_cpu_freq_calculated + 3)
	LD	E, A
	LD	BC, 100
	XOR	A
	CALL	__ldivu
	LD	(IX-9), HL				; HL = _cpu_freq_calculated / 100

	LD	BC, (IX-3)				; BC = DESIRED CS3 BUS FREQ
	CALL	__stoiu
	LD	BC, HL					; CONVERTED TO 24 BITS

	XOR	A
	LD	HL, (IX-9)				; HL = _cpu_freq_calculated / 100
	CALL	__ldivu					; HL = HL / BC

	LD	A, 5					; HL = HL + 5
	CALL	__ladd_b

	LD	BC, 10					; HL = HL / 10
	XOR	A
	CALL	__ldivu
	LD	(IX-10), L				; L = CALCULATED CS3 BUS CYCLES

	LD	BC, (IX-6)				; DESIRED CS2 BUS FREQ
	CALL	__stoiu
	LD	BC, HL					; CONVERTED TO 24 BITS

	XOR	A
	LD	HL, (IX-9)				; _cpu_freq_calculated / 100
	CALL	__ldivu

	LD	A, 5					; HL = HL + 5
	CALL	__ladd_b

	LD	BC, 10					; HL = HL / BC
	XOR	A
	CALL	__ldivu					; L = CALCULATED CS3 BUS CYCLES

	LD	H, (IX-10)				; CALCULATED CS3 BUS CYCLES

	LD	SP, IX
	POP	IX
	JR	ez80_set_bus_cycles
