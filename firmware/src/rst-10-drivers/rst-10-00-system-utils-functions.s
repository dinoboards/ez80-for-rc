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
	INCLUDE "..\version.inc"

	XREF	__ladd_b
	XREF	__ldivu
	XREF	__stoiu
	XREF	_cpu_freq_calculated
	XREF	_cs_bus_timings
	XREF	_platform_description
	XREF	_SYS_CLK_FREQ
	XREF	_system_ticks
	XREF	_calculate_wait_state
	XREF	remove_usb_tick_hook
	XREF	send_int_to_z80
	XREF	tmr_irq

	XREF	_util_get_day_of_month
	XREF	_util_get_month
	XREF	_util_get_year

	XREF	_ez80_exchange_version_verified

	SECTION CODE

	.assume adl=1

	PUBLIC	_system_led_timer_isr
	XREF	_led_flashing_state
	XREF	_led_flash_delay_count
	XREF	_led_flash_delay_period

; interrupt to control flash rate of on board led
_system_led_timer_isr:
	push	hl
	push	de
	push	af

	IN0    A, (TMR3_CTL)				; CLEAR INTERRUPT SIGNAL

	xor	a
	sbc	hl, hl
	ex	hl, de
	LD	HL, (_led_flash_delay_count)
	dec	hl
	ld	(_led_flash_delay_count), hl
	or	a
	sbc	hl, de
	jr	nz, led_continue

	push	bc
	ld	hl, (_led_flash_delay_period)
	ld	(_led_flash_delay_count), hl
	LD	HL, _led_flashing_state
	LD	A, (HL)
	XOR	%01
	LD	(HL), A
	LD	B, A
	IN	A, (PC_DR)
	AND	%FE
	OR	B
	OUT0	(PC_DR), A
	pop	bc

led_continue:
	pop	af
	pop	de
	pop	hl
	ei
	ret.l

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
	JR	Z, ez80_mem0_bus_timing_set		; B = 1, SYSUTL_MEM0TM_SET
	DEC	A
	JR	Z, ez80_mem1_bus_timing_set		; B = 2, SYSUTL_MEM1TM_SET
	DEC	A
	JR	Z, not_supported			; B = 3, DEPRECATED
	DEC	A
	JR	Z, not_supported			; B = 4, DEPRECATED
	DEC	A
	JR	Z, ez80_cpu_freq_get			; B = 5, SYSUTL_CPU_FREQ_GET
	DEC	A
	JR	Z, not_supported			; B = 6, DEPRECATED
	DEC	A
	JR	Z, ez80_debug				; B = 7, SYSUTL_DEBUG
	DEC	A					; B = 8, SYSUTL_MEMTM_SET (CS3)
	JR	Z, ez80_mem_bus_timing_set
	DEC	A					; B = 9, SYSUTL_IOTM_SET
	JR	Z, ez80_io_bus_timing_set
	DEC	A					; B = 10, SYSUTL_MEMTM_GET (CS3, CS1, CS0)
	JR	Z, ez80_mem_bus_timing_get
	DEC	A					; B = 11, SYSUTL_IOTM_GET
	JR	Z, ez80_io_bus_timing_get
	DEC	A					; B = 12, SYSUTL_MEMTMFQ_SET (CS3)
	JR	Z, ez80_mem_bus_timing_freq_set
	DEC	A					; B = 13, SYSUTL_IOTMFQ_SE
	JR	Z, ez80_io_bus_timing_freq_set
	DEC	A					; B = 14, SYSUTL_FLASHWS_SET
	JR	Z, ez80_flash_ws_set
	DEC	A					; B = 15, SYSUTL_FLASHWS_GET
	JR	Z, ez80_flash_ws_get
	DEC	A					; B = 16, SYSUTL_FLSHFQ_SET
	JR	Z, ez80_flsh_freq_set
	DEC	A
	JR	Z, ez80_mem0_bus_timing_freq_set	; B = 17, SYSUTL_MEMTMFQ_SET (CS0)
	DEC	A
	JR	Z, ez80_mem1_bus_timing_freq_set	; B = 18, SYSUTL_MEMTMFQ_SET (CS1)
	DEC	A
	JR	Z, ez80_emulation_state_get		; B = 19, SYSUTL_EMULSTAT_GET
	DEC	A
	JR	Z, q3vm_create				; B = 20, SYSUTL_Q3VM_CREATE
	DEC	A
	JR	Z, q3vm_call				; B = 20, SYSUTL_Q3VM_CALL
	DEC	A
	JR	Z, q3vm_set_stack			; B = 21, SYSUTL_Q3VM_SET_STACK

not_supported:
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
;  A	 = 0 -> Success, otherwise errored
;  C	 = eZ80 Chip Id (0->EZ80F92AZ020EG)
;  D	 = eZ80 Firmware Major Version Number
;  E	 = eZ80 Firmware Minor Version Number
;  u(HL) = eZ80 Firmware Patch Version Number
;  HL	 = eZ80 Firmware Revision Version Number
;  C'	 = firmware's build day of month
;  D'	 = firmware's build month
;  E'	 = firmware's build year (20xx)
;
ez80_version_exchange:

	; RomWBW has re-initialised - so remove the usb timer tick hook
	PUSH	HL
	CALL	remove_usb_tick_hook
	POP	HL

	; Disable External interrupt signals
	XOR	A
	LD	(send_int_to_z80), A
	LD	(tmr_irq), A

	; Double rate of LED FLASHING
	LD	HL, (_led_flash_delay_period)
	ADD	HL, HL
	LD	(_led_flash_delay_period), HL

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
	LD	DE, MAJOR_VERSION << 8 | MINOR_VERSION
	LD	HL, PATCH_VERSION << 8 | REVISION_VERSION
	LD	C, 0

	XOR	A
	RET.L
;
; Function B = 01 -- SYSUTL_MEM0TM_SET
; Set Bus cycles or wait state for external linear memory access
; using CS0
;
; Input
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;
; Output
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A 	= 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_mem0_bus_timing_set:
	LD	A, L

	BIT	7, A
	JR	NZ, ez80_mem0_bus_z80_mode

	AND	%07
	LD	(_mem0_bus_mode_and_timing), A

	RLCA
	RLCA
	RLCA
	RLCA
	RLCA
	OR	CSX_TYPE_MEM | CSX_ENABLED
	OUT0	(CS0_CTL), A

	LD	A, BMX_BM_EZ80 | BMX_AD_SEPARATE
	OUT0	(CS0_BMC), A

ez80_mem0_bus_timing_set_done:
	LD	A, (_mem0_bus_mode_and_timing)
	LD	L, A
	XOR	A
	RET.L

ez80_mem0_bus_z80_mode:
	AND	%8F
	LD	(_mem0_bus_mode_and_timing), A

	; Assign CS3 bus mode
	AND	%0F
	OR	BMX_BM_Z80 | BMX_AD_SEPARATE
	OUT0	(CS0_BMC), A

	JR	ez80_mem0_bus_timing_set_done

; Function B = 02 -- SYSUTL_MEM1TM_SET
; Set Bus cycles or wait state for external linear memory access
; using CS1
;
; Input
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;
; Output
;   L 	= BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A 	= 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_mem1_bus_timing_set:
	LD	A, L

	BIT	7, A
	JR	NZ, ez80_mem1_bus_z80_mode

	AND	%07
	LD	(_mem1_bus_mode_and_timing), A

	RLCA
	RLCA
	RLCA
	RLCA
	RLCA
	OR	CSX_TYPE_MEM | CSX_ENABLED
	OUT0	(CS1_CTL), A

	LD	A, BMX_BM_EZ80 | BMX_AD_SEPARATE
	OUT0	(CS1_BMC), A

ez80_mem1_bus_timing_set_done:
	LD	A, (_mem1_bus_mode_and_timing)
	LD	L, A
	XOR	A
	RET.L

ez80_mem1_bus_z80_mode:
	AND	%8F
	LD	(_mem1_bus_mode_and_timing), A

	; Assign CS3 bus mode
	AND	%0F
	OR	BMX_BM_Z80 | BMX_AD_SEPARATE
	OUT0	(CS1_BMC), A

	JR	ez80_mem1_bus_timing_set_done
;
; Function B = 03 -- DEPRECATED
;
;
; Input
;
; Output
;

;
; Function B = 04 --DEPRECATED
;
; Input
;
; Output
;


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
; Function B = 06 -- DEPRECATED
;
; Inputs:
;
; Outputs:
;

;
; Function B = 07 -- SYSUTL_DEBUG
; Convenience function to allow breakpoint to be set within ZDS
; to debug through external memory code
;
ez80_debug:
	RET.L

	XREF	_mem_bus_timings
	XREF	_mem_bus_mode_and_timing
	XREF	_mem0_bus_mode_and_timing
	XREF	_mem1_bus_mode_and_timing
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

	LD	A, BMX_BM_EZ80 | BMX_AD_SEPARATE
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
	OR	BMX_BM_Z80 | BMX_AD_SEPARATE
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

	LD	A, BMX_BM_EZ80 | BMX_AD_SEPARATE
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
	OR	BMX_BM_Z80 | BMX_AD_SEPARATE
	OUT0	(CS2_BMC), A

	JR	ez80_io_bus_timing_set_done

;
; Function B = 10 -- SYSUTL_MEMTM_GET
; Get Bus cycles or wait state for external memory access
; using CS3
;
; Output
;   L 	= MODE for CS3 (main memory) BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   H   = Mode for CS0 (extended memory) BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   u	= Mode for CS1 (extended memory) BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A 	= 0 -> SUCCESS
;
ez80_mem_bus_timing_get:
	LD	HL, (_mem_bus_timings)
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
	LD	A,%80
	OR	E 	; must be B/C
	LD	E, A
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
;   uHL = min access time in nano seconds
;   E => hard minimum number of w/s
;
; Output
;   L  = BIT 7 = BUS MODE - 0 = EZ80, 1 = Z80, BITS 0 to 2 = cycles or wait state
;   A  = 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_io_bus_timing_freq_set:
	LD	A,%80
	OR	E 	; must be B/C
	LD	E, A
	PUSH	DE
	PUSH	HL
	CALL	_calculate_wait_state
	POP	HL
	POP	DE
	LD	L, A
	JP	ez80_io_bus_timing_set
;
; Function B = 14 -- SYSUTL_FLASHWS_SET
; Set the wait state for the on-chip flash memory access
; Default on boot up in 4 wait states
;
; Input
;   L 	= wait state
;
; Output
;   L 	= wait state
;   A 	= 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_flash_ws_set:
	LD	A, L
	AND	7
	LD	L, A

	RLCA
	RLCA
	RLCA
	RLCA
	RLCA
	OR	%08		; keep flash enabled bit set

	OUT0	(FLASH_CTRL), A
	XOR	A
	RET.L
;
; Function B = 15 -- SYSUTL_FLASHWS_GET
; Get the wait state for the on-chip flash memory access
;
; Input
;   None
;
; Output
;   L 	= wait state
;   A 	= 0 -> SUCCESS
;
ez80_flash_ws_get:
	IN0	L, (FLASH_CTRL)
	LD	A, L
	RRCA
	RRCA
	RRCA
	RRCA
	RRCA
	AND	7
	LD	L, A
	XOR	A
	RET.L

;
; Function B = 16 -- SYSUTL_FLSHFQ_SET
; Identify and apply the appropriate W/S to the on-chip flash memory access
; Zilog specified on-chip ROM needs 60ns at least (but this can typically be exceeded)
;
; Input
;   uHL = min access time in nano seconds
;
; Output
;   L  = wait state
;   A  = 0 -> SUCCESS, NZ -> VALUE OUT OF RANGE
;
ez80_flsh_freq_set:
	LD	DE, 0	; min 0 wait states allow
	PUSH	DE
	PUSH	HL
	CALL	_calculate_wait_state
	POP	HL
	POP	DE
	LD	L, A
	JR	ez80_flash_ws_set
;
; Function B = 17 -- SYSUTL_MEMTMFQ_SET (CS0)
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
ez80_mem0_bus_timing_freq_set:
	PUSH	DE
	PUSH	HL
	CALL	_calculate_wait_state
	POP	HL
	POP	DE
	LD	L, A
	JP	ez80_mem0_bus_timing_set
;
; Function B = 18 -- SYSUTL_MEMTMFQ_SET (CS1)
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
ez80_mem1_bus_timing_freq_set:
	PUSH	DE
	PUSH	HL
	CALL	_calculate_wait_state
	POP	HL
	POP	DE
	LD	L, A
	JP	ez80_mem1_bus_timing_set

;
; Function B = 19 SYSUTL_EMULSTAT_GET
; Return state of caller's Z80 execution environment
;
; if function invoked in ADL mode, will return 1: Native Environment
; typically call this function via rst.l within z80 or emulated modes
;
; Output
;  A = 01 -> Native Environment
;  A = 02 -> Emulated Environment
;
ez80_emulation_state_get:
	; the emulator will intercept this request
	; as such, a hard coded value works
	ld	a, 1
	ret.l



;
; Function B = 20 SYSUTL_Q3VM_CREATE
; Initialise a new VM instance
;
; Must only be called from ADL mode using RST %10 (no .L suffix)
;
; Invokes C function:
;
; bool VM_Create(vm_t                *vm,
;               const uint8_t *const bytecode,
;               const vm_size_t      length,
;               uint8_t *const       dataSegment,
;               const vm_size_t      dataSegmentLength,
;               uint32_t (*systemCalls)(vm_t *, uint8_t *));
;
; Input:
;
; Output
;  A = -1 if error
;  A =  0 if success
;
	XREF	_VM_Create
q3vm_create:
	JP	_VM_Create

;
; Function B = 21 SYSUTL_Q3VM_CALL1
; Initialise a new VM instance
;
; Must only be called from ADL mode using RST %10 (no .L suffix)
;
; Invokes C function:
;
; uint32_t VM_Call(vm_t *vm, uint24_t command, ...);
;
; Input:
;
; Output
;  ?? 32 bit result
;
	XREF	_VM_Call
q3vm_call:
	JP	_VM_Call

	XREF	_VM_SetStackStore
q3vm_set_stack:
	JP	_VM_SetStackStore
