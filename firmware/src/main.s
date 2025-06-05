	include	"config.inc"

	section	CODE

	.assume	adl=1

	public	_main
	xref	_uart0_init
	xref	_rx_buffer_init
	xref	_init_clocks
	xref	__c_startup

	xref	_io_bus_mode_and_timing
	xref	_mem_bus_mode_and_timing
	xref	_mem0_bus_mode_and_timing
	xref	_mem1_bus_mode_and_timing

	xref	probe_for_msx_system
	xref	z80_invoke

_main:
	call	remove_usb_tick_hook
	call	__c_startup
	call	_init_clocks
	call	_rx_buffer_init
	call	_uart0_init

	; Align stored timings to actual configured startup timings.
	ld	a, IO_BUS_CYCLES|%80
	ld	(_io_bus_mode_and_timing), a

	ld	a, MEM_BUS_CYCLES|%80
	ld	(_mem_bus_mode_and_timing), a

	ld	a, MEM_BUS_CYCLES|%80
	ld	(_mem0_bus_mode_and_timing), a

	ld	a, MEM_BUS_CYCLES|%80
	ld	(_mem1_bus_mode_and_timing), a

	; Auto configure CS0's timing
	ld	a, 0
	ld	b, 17
	ld	hl, 70 ; 80ns
	ld	e, 1
	RST.L	%10

	; Auto configure CS1's timing
	ld	a, 0
	ld	b, 18
	ld	hl, 30 ; 80ns
	ld	e, 1
	RST.L	%10

IFDEF	ZEXALL
	xref	Z80test
	call	Z80test
ENDIF

	;xref	_spike
	;call	_spike

	ld	a, Z80_ADDR_MBASE		; set MBASE to $03
	ld	MB, a

	call	probe_for_msx_system
	jp.sis	nz, 0

	jp	z80_invoke

	global	remove_usb_tick_hook
	; remove the usb key/mouse inter handler hook
remove_usb_tick_hook:
	xref	_system_timer_isr
	ld	hl, _system_timer_isr
	xor	a
	ld	(HL), a
	inc	hl
	ld	(HL), a
	inc	hl
	ld	(HL), a
	inc	hl
	ld	(HL), a
	inc	hl
	ret
