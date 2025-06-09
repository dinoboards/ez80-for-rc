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

	xref	z80_invoke

_main:
	call	remove_usb_tick_hook
	call	__c_startup
	call	_init_clocks
	call	_rx_buffer_init
	call	_uart0_init
	call	_init_memory_timings

IFDEF	ZEXALL
	xref	Z80test
	call	Z80test
ENDIF

	;xref	_spike
	;call	_spike

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

_init_memory_timings:
	; Auto configure CS0's timing
	ld	a, 0
	ld	b, 17
	ld	hl, 70			; 70ns
	ld	e, 1
	rst.l	%10

	; Auto configure CS1's timing
	ld	a, 0
	ld	b, 18
	ld	hl, 30			; 30ns
	ld	e, 1
	rst.l	%10

	; set main mem to 2bc (for 32mhz)
	xor	a
	ld	b, 12			; SYSUTL_MEMTMFQ_SET
	ld	hl, 180			; 180ns
	ld	e, %80			; must be B/C
	rst.l	%10			; but can be 1 (25mhz) for msx-dos

       ; set io to 5bc (for 32mhz)
	xor	a
	ld	b, 13			; SYSUTL_IOTMFQ_SET
	ld	hl, 320			;
	ld	e, %80			; must be B/C
	rst.l	%10			; but can be 4 (25mhz) for msx-dos

	; set flash to 1ws (for 32mhz)
	xor	a
	ld	b, 16			; SYSUTL_FLSHFQ_SET
	ld	hl, 60
	rst.l	%10

	ret
