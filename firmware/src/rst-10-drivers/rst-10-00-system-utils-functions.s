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
	;JR	Z, ...					; B = 1, DEPRECATED
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
	LD	H, 0
	LD	L, 0

IFDEF RC2014_ALT_FIRMWARE
	LD	C, %80
ELSE
	LD	C, 0
ENDIF

	XOR	A
	RET.L
;
; Function B = 01 -- DEPRECATED
;
;
; Input
;
;
; Output
;

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
