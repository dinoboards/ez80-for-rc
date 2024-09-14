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
	XREF	_calculate_wait_state

	XREF	_util_get_day_of_month
	XREF	_util_get_month
	XREF	_util_get_year

	XREF	_ez80_exchange_version_verified

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
	JR	Z, ez80_bus_cycles_get			; B = 1, SYSUTL_BUSTM_GET
	DEC	A
	;JR	Z, ...					; B = 2, DEPRECATED
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

	; temp new version of bus timing config functions
	; will be swapped to indexes above, once full HBIOS conversion is completed

	DEC	A					; B = 8, SYSUTL_MEMTM_SET
	JR	Z, ez80_mem_bus_timing_set
	DEC	A					; B = 9, SYSUTL_IOTM_SET
	JR	Z, ez80_io_bus_timing_set
	DEC	A					; B = 10, SYSUTL_MEMTM_GET
	JR	Z, ez80_mem_bus_timing_get
	DEC	A					; B = 11, SYSUTL_IOTM_GET
	JR	Z, ez80_io_bus_timing_get
	DEC	A					; B = 12, SYSUTL_MEMTMFQ_SET
	JR	Z, ez80_mem_bus_timing_freq_set
	DEC	A					; B = 13, SYSUTL_IOTMFQ_SET
	JR	Z, ez80_io_bus_timing_freq_set

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
;  C	= eZ80 Chip Id (0->EZ80F92AZ020EG), Bit 7 is set if running on alt-firmware
;  D	= eZ80 Firmware Major Version Number
;  E	= eZ80 Firmware Minor Version Number
;  H	= eZ80 Firmware Patch Version Number
;  L	= eZ80 Firmware Revision Version Number
;  C'	= firmware's build day of month
;  D'	= firmware's build month
;  E'	= firmware's build year (20xx)


ez80_version_exchange:
IFDEF RC2014_ALT_FIRMWARE
	CALL	_ez80_exchange_version_verified
ENDIF
	CALL	_util_get_day_of_month
	LD	C, A
	PUSH	BC
	CALL	_util_get_month
	LD	D, A
	PUSH	DE
	CALL	_util_get_year
	POP	DE
	POP	BC
	LD	E, A
	EXX
	LD	D, 0
	LD	E, 1
	LD	H, 1
	LD	L, 0

IFDEF RC2014_ALT_FIRMWARE
	LD	C, %80
ELSE
	LD	C, 0
ENDIF

	XOR	A
	RET.L
;
; Function B = 01 -- SYSUTL_BUSTM_GET
; Get Bus cycles for external memory and I/O
;
; Input
;  None
;
; Output
; H	= CS3 External Memory Bus Cycles (1-15)
; L	= CS2 External I/O Bus Cycles (1-15)
; DE	= 0
; A	= 0 -> SUCCESS
ez80_bus_cycles_get:
	LD	HL, (_cs_bus_timings)
	LD	DE, 0
	XOR	A
	RET.L

; Function B = 02 -- DEPRECATED
;
;
; Input
;
;
; Output
;
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

	XOR	A					; IO must always be >= 1 & <= 15
	OR	L
	JR	NZ, valid_io_bus_cycle
	LD	L,1
valid_io_bus_cycle:

	XOR	A					; MEM must always be >= 1 & <= 15
	OR	H
	JR	NZ, valid_mem_bus_cycle
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
; Set Bus cycles to based on an equivalent frequency calculation.
; If the calculated values are below the supplied minimum values (H', L')
; the minimum values are used.
;
; Input
;   HL = CS3 External Memory Bus Frequency (Khz)
;   DE = CS2 External I/O Bus Frequency (Khz)
;   H' = Minimum CS3 External Memory Bus Cycles (1-15)
;   L' = Minimum CS2 External I/O Bus Cycles (1-15)
;
; Output
;   H = CS3 External Memory Bus Cycles (1-15)
;   L = CS2 External I/O Bus Cycles (1-15)
;   A = 0 -> SUCCESS, NZ -> ONE OR MORE VALUES OUT OF RANGE
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

	EXX
	LD	A, L					; IO must always be >= L'
	EXX
	CP	L
	JR	C, valid_io_fq
	LD	L, A
valid_io_fq:
	EXX
	LD	A, H
	EXX
	CP	H					; Mem must always be >= H'
	JR	C, valid_mem_fq
	LD	H, A
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
	RET.L

	XREF	_mem_bus_mode_and_timing
	XREF	_io_bus_mode_and_timing
;
; Function B = 08 -- SYSUTL_MEMTM_SET
; Set Bus cycles or wait state for external memory access
; using CS3
;
; Input
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;
; Output
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A 	= 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_mem_bus_timing_set:
	LD	A, L

	BIT	7, A
	JR	NZ, ez80_mem_bus_z80_mode

	AND	%07
	LD	(_mem_bus_mode_and_timing), A

	RLCA
	RLCA
	RLCA
	RLCA
	RLCA
	OR	CSX_TYPE_MEM | CSX_ENABLED
	OUT0	(CS3_CTL), A

	LD	A, BMX_BM_EZ80 | BMX_AD_SEPERATE
	OUT0	(CS3_BMC), A

ez80_mem_bus_timing_set_done:
	LD	A, (_mem_bus_mode_and_timing)
	LD	L, A
	XOR	A
	RET.L

ez80_mem_bus_z80_mode:
	AND	%8F
	LD	(_mem_bus_mode_and_timing), A

	; Assign CS3 bus mode
	AND	%0F
	OR	BMX_BM_Z80 | BMX_AD_SEPERATE
	OUT0	(CS3_BMC), A

	JR	ez80_mem_bus_timing_set_done

;
; Function B = 09 -- SYSUTL_IOTM_SET
; Set Bus cycles or wait state for external memory access
; using CS2
;
; Input
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;
; Output
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A 	= 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_io_bus_timing_set:
	LD	A, L

	BIT	7, A
	JR	NZ, ez80_io_bus_z80_mode

	AND	%07
	LD	(_io_bus_mode_and_timing), A

	RLCA
	RLCA
	RLCA
	RLCA
	RLCA
	OR	CSX_TYPE_IO | CSX_ENABLED
	OUT0	(CS2_CTL), A

	LD	A, BMX_BM_EZ80 | BMX_AD_SEPERATE
	OUT0	(CS2_BMC), A

ez80_io_bus_timing_set_done:
	LD	A, (_io_bus_mode_and_timing)
	LD	L, A
	XOR	A
	RET.L

ez80_io_bus_z80_mode:
	AND	%8F
	LD	(_io_bus_mode_and_timing), A

	; Assign CS2 bus mode
	AND	%0F
	OR	BMX_BM_Z80 | BMX_AD_SEPERATE
	OUT0	(CS2_BMC), A

	JR	ez80_io_bus_timing_set_done

;
; Function B = 10 -- SYSUTL_MEMTM_GET
; Get Bus cycles or wait state for external memory access
; using CS3
;
; Output
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A 	= 0 -> SUCCESS
;
ez80_mem_bus_timing_get:
	LD	A, (_mem_bus_mode_and_timing)
	LD	L, A
	XOR	A
	RET.L

;
; Function B = 11 -- SYSUTL_IOTM_GET
; Get Bus cycles or wait state for external memory access
; using CS2
;
; Output
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A 	= 0 -> SUCCESS
;
ez80_io_bus_timing_get:
	LD	A, (_io_bus_mode_and_timing)
	LD	L, A
	XOR	A
	RET.L
;
; Function B = 12 -- SYSUTL_MEMTMFQ_SET
; Identify and apply the appropriate W/S or switch to BUS cycles
; to ensure a memory operation is given a minimum cycle duration.
;
; Input
;   uHL = min cycle duration in nano seconds
;   E => hard minimum number of w/s
;
; Output
;   L  = BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A  = 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_mem_bus_timing_freq_set:
	PUSH	DE
	PUSH	HL
	CALL	_calculate_wait_state
	POP	HL
	POP	DE
	LD	L, A
	JP	ez80_mem_bus_timing_set
;
; Function B = 13 -- SYSUTL_IOTMFQ_SET
; Identify and apply the appropriate W/S or switch to BUS cycles
; to ensure an I/O operation is given a minimum cycle duration.
;
; Input
;   uHL = min cycle duration in nano seconds
;   E => hard minimum number of w/s
;
; Output
;   L  = BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A  = 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_io_bus_timing_freq_set:
	PUSH	DE
	PUSH	HL
	CALL	_calculate_wait_state
	POP	HL
	POP	DE
	LD	L, A
	JP	ez80_io_bus_timing_set
