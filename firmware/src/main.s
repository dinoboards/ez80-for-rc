	include	"config.inc"

	section	CODE

	.assume	adl=1

	public	_main
	XREF	_uart0_init
	XREF	_rx_buffer_init
	XREF	_init_clocks
	XREF	__c_startup
	; XREF	_vm_init

	XREF	_io_bus_mode_and_timing
	XREF	_mem_bus_mode_and_timing
	XREF	_mem0_bus_mode_and_timing
	XREF	_mem1_bus_mode_and_timing

	XREF	_boot_prompt1
	XREF	_boot_prompt2
	XREF	_probe_installed_linear_ram
	XREF	_report_installed_linear_ram
	XREF	_probe_extrom
	XREF	_report_extrom

	XREF	_allocate_ram_for_hbios_and_fbios
	XREF	_report_seg_for_cpm

	XREF	_hbios_cio_init
	XREF	_hbios_dio_init
	XREF	_report_hbios_drivers
	XREF	_os_boot

	XREF	z80_invoke

_main:
	call	remove_usb_tick_hook
	call	__c_startup
	call	_init_clocks
	call	_rx_buffer_init
	call	_uart0_init
	call	_init_memory_timings
	; CALL	_vm_init

	call	_boot_prompt1

	CALL	_probe_installed_linear_ram
	LD	L, A
	PUSH	HL

	CALL	_probe_extrom
	LD	L, A

	POP	DE

	; XREF	_spike
	; call	_spike

	LD	A, L

	CP	1
	JR	Z, boot_romwbw_rom
	CP	2
	JR	Z, boot_msx_rom

	LD	A, E
	OR	A
	JR	Z, boot_external_rom


boot_storage:
	CALL	_boot_prompt2

	CALL	_allocate_ram_for_hbios_and_fbios
	CALL	_hbios_cio_init
	CALL	_hbios_dio_init
  	CALL	_report_hbios_drivers
	CALL	_os_boot

IFDEF	ZEXALL
	XREF	Z80test
	call	Z80test
ENDIF

boot_external_rom:
boot_msx_rom:
	DI
	JP	z80_invoke

boot_romwbw_rom:
	LD	A, Z80_ADDR_MBASE
	LD	MB, A
	JP.SIS	0


	GLOBAL	remove_usb_tick_hook
	; remove the usb key/mouse inter handler hook
remove_usb_tick_hook:
	XREF	_system_timer_isr
	LD	HL, _system_timer_isr
	XOR	A
	LD	(HL), A
	INC	HL
	LD	(HL), A
	INC	HL
	LD	(HL), A
	INC	HL
	LD	(HL), A
	INC	HL
	RET

_init_memory_timings:
	; Auto configure CS0's timing
	ld	a, 0
	ld	b, 17
	ld	hl, MEM_CS0_TIMING
	ld	e, 1
	rst.l	%10

	; Auto configure CS1's timing
	ld	a, 0
	ld	b, 18
	ld	hl, MEM_CS1_TIMING
	ld	e, 1
	rst.l	%10

	; set main mem to 2bc (for 32mhz)
	xor	a
	ld	b, 12			; SYSUTL_MEMTMFQ_SET
	ld	hl, MEM_CS3_TIMING
	ld	e, %80			; must be B/C
	rst.l	%10			; but can be 3 (25mhz) for pacman


       ; set io to 5bc (for 32mhz)
	xor	a
	ld	b, 13			; SYSUTL_IOTMFQ_SET
	ld	hl, IO_CS2_TIMING
	ld	e, %84			; must be B/C and at least 4bc
	rst.l	%10			; but can be 4 (25mhz) for msx-dos

	; set flash to 1ws (for 32mhz)
	xor	a
	ld	b, 16			; SYSUTL_FLSHFQ_SET
	ld	hl, MEM_FLSH_TIMING
	rst.l	%10

	ret
