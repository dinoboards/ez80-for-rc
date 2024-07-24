;
; General System Utilities Functions
;
; Access with RST.L %10 (A=0, B=sub function code)
;
; The System Utilities provides functions to access the system configuration
; and control functions, including the ability to query the firmware version,
; and configure the eZ80 bus cycles for external memory and I/O.
;
        INCLUDE "..\config.inc"

	XREF	__ladd_b
	XREF	__ldivu
	XREF	__stoiu
	XREF	_cpu_freq_calculated
	XREF	_cs_bus_timings
	XREF	_platform_description
	XREF	_SYS_CLK_FREQ
	XREF	_system_ticks

	SECTION CODE

	.assume adl=1

	PUBLIC	_system_utils_dispatch
;
; SYSTEM UTILS DISPATCH
; Dispatcher for the RST.L %10 trap functions
;
; Inputs:
;  B      = Utils sub function index
;
; Outputs:
;  A	 = 0 -> Success, otherwise errored
;  Other registers as per sub-functions
;
_system_utils_dispatch:
	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, ez80_version_exchange		; B = 0, SYSUTL_VER_EXCHANGE
	DEC	A
	JR	Z, ez80_reg_ehl_to_hl			; B = 1, SYSUTL_EHL_TO_HL
	DEC	A
	JR	Z, ez80_reg_hl_to_ehl			; B = 2, SYSUTL_HL_TO_EHL
	DEC	A
	JR	Z, ez80_bus_cycles_set			; B = 3, SYSUTL_BUSTM_SET
	DEC	A
	JR	Z, ez80_bus_freq_set			; B = 4, SYSUTL_BUSFQ_SET
	DEC	A
	JR	Z, ez80_cpu_freq_get			; B = 5, SYSUTL_CPU_FREQ_GET
	DEC	A
	JR	Z, ez80_bank_helper_set			; B = 6, SYSUTL_BANK_HELPER_SET
	DEC	A
	JR	Z, ez80_debug				; B = 7, SYSUTL_DEBUG

	LD	A, %FF					; UNKNOWN FUNCTION
	RET.L
;
; Function B = 00 -- SYSUTL_VER_EXCHANGE
; Exchange the firmware version information with the eZ80 firmware and the operating platform
;
; Inputs:
;  C	= Platform Type Code (1 - RomWBW, 2 - Yellow MSX, 255 -> Other)
;  D	= Major Version Number
;  E	= Minor Version Number
;  H	= Patch Version Number
;  L	= Revision Version Number
;
; Outputs:
;  A	= 0 -> Success, otherwise errored
;  C	= eZ80 Chip Id (0->EZ80F92AZ020EG)
;  D	= eZ80 Firmware Major Version Number
;  E	= eZ80 Firmware Minor Version Number
;  H	= eZ80 Firmware Patch Version Number
;  L	= eZ80 Firmware Revision Version Number
;
ez80_version_exchange:
	LD	D, 0
	LD	E, 1
	LD	H, 0
	LD	L, 0
	LD	C, 0

	XOR	A
	RET.L
;
; Function B = 01 -- SYSUTL_HL_TO_EHL
; register copy (8:16 -> 24) -- LD HL, E:HL
;
; Input
;   E:HL	= 24 bit value to be copied
;
; Output
;   uHL		= E:HL
;
; not re-entrant safe
;
ez80_reg_ehl_to_hl:
	LD	(tmp), HL				; ONLY THE LOWER 16 BITS OF HL ARE VALID
	LD	A, E
	LD	(tmp+2), A				; SAVE THE UPPER 8 BITS OF THE 24 BIT VALUE
	LD	HL, (tmp)
	RET.L

; Function B = 02 -- SYSUTL_HL_TO_EHL
; register copy (24 -> 8:16) -- LD E:HL, HL
;
; Input
;   uHL 	= 24 bit value to be copied
;
; Output
;   E:HL	= uHL
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
;
; Function B = 03 -- SYSUTL_BUSTM_SET
; Set Bus cycles for external memory and I/O
;
; Input
;   H 	= CS3 External Memory Bus Cycles (1-15)
;   L 	= CS2 External I/O Bus Cycles (1-15)
;
; Output
;   H 	= CS3 External Memory Bus Cycles (1-15)
;   L 	= CS2 External I/O Bus Cycles (1-15)
;   A 	= 0 -> SUCCESS, NZ -> ONE ORE MORE VALUES OUT OF RANGE
;
ez80_bus_cycles_set:
	PUSH	IX
	LD	IX, _cs_bus_timings

	LD	A, 2					; IO must always be >= 2 & <= 15
	CP	L
	JR	C, valid_io_bus_cycle
	LD	L, 2
valid_io_bus_cycle:

	CP	H
	JR	C, valid_mem_bus_cycle
	LD	H, 1
valid_mem_bus_cycle:

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
;
; Function B = 04 --SYSUTL_BUSFQ_SET
; Set Bus cycles to based on an equivalent frequency calculation
;
; Input
;   HL = CS3 External Memory Bus Frequency (Khz)
;   DE = CS2 External I/O Bus Frequency (Khz)
;
; Output
;   H = CS3 External Memory Bus Cycles (1-15)
;   L = CS2 External I/O Bus Cycles (1-15)
;   A = 0 -> SUCCESS, NZ -> ONE ORE MORE VALUES OUT OF RANGE
;
ez80_bus_freq_set:
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
	CALL	__ldivu					; L = CALCULATED CS2 BUS CYCLES

	LD	H, (IX-10)				; CALCULATED CS3 BUS CYCLES

	LD	A, 2					; IO must always be >= 2 & <= 15
	CP	L
	JR	C, valid_io_fq
	LD	L, 2
valid_io_fq:
	DEC	A
	CP	H					; Mem must always be >= 1 & <= 15
	JR	C, valid_mem_fq
	LD	H, 1
valid_mem_fq:


	LD	SP, IX
	POP	IX
	JR	ez80_bus_cycles_set
;
; Function B = 05 -- SYSUTL_CPU_FREQ_GET
; Get CPU Frequency
;
; Output
;  AuBC = CPU Frequency (32 bits)
;  HL  	= CPU Frequency in Khz (16 bits)
;  E	= CPU Frequency in Mhz (8 bits)
;  A  = 0 -> Success, otherwise errored
;
ez80_cpu_freq_get:
	LD	HL, (_cpu_freq_calculated)
	LD	A, (_cpu_freq_calculated+3)
	LD	E, A
	LD	BC, 1000000
	XOR	A
	CALL	__ldivu					; L = CPU Frequency in Mhz
	PUSH	HL

	LD	HL, (_cpu_freq_calculated)
	LD	A, (_cpu_freq_calculated+3)
	LD	E, A
	LD	BC, 1000
	XOR	A
	CALL	__ldivu					; HL = CPU Frequency in Khz

	POP	DE					; E = CPU Frequency in Mhz

	LD	BC, (_cpu_freq_calculated)
	LD	A, (_cpu_freq_calculated+3)

	XOR	A
	RET.L
;
; Function B = 06 -- SYSUTL_BANK_HELPER_SET
;
; Set the type of memory banking used for the external memory access
; This function will control the banking operations for RST %18 helper
;
; Only support memory banking model supported is RomWBW(1)/MM_Z2(2)
;
; Inputs:
;  C 	= Banking type -(Code as per platform type)
;  HL	= Rom KB size
;  DE	= Ram KB size
;
; Outputs:
; A 	= 0 -> SUCCESS, NZ -> helper support not available for requested platform
;
ez80_bank_helper_set:
	LD	A, C
	CP	ROMWBW_MM_Z2
	JR	NZ, ez80_bank_helper_set_not_supported

	XOR	A				; SUCCESS
	RET.L

ez80_bank_helper_set_not_supported:
	LD	A, 1
	OR	A
	RET.L
;
; Function B = 07 -- SYSUTL_DEBUG
; Convenience function to allow breakpoint to be set within ZDS
; to debug through external memory code
;
ez80_debug:
	XOR	A
	RET.L
