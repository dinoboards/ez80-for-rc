	INCLUDE	"config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	_main
	XREF	_uart0_init
	XREF	_rx_buffer_init
	XREF	_init_clocks
	XREF	__c_startup

	XREF	_io_bus_mode_and_timing
	XREF	_mem_bus_mode_and_timing
	XREF	_mem0_bus_mode_and_timing
	XREF	_mem1_bus_mode_and_timing

_main:
	CALL	remove_usb_tick_hook
	CALL	__c_startup
	CALL	_init_clocks
	CALL	_rx_buffer_init
	CALL	_uart0_init

	; Align stored timings to actual configured startup timings.
	LD	A, IO_BUS_CYCLES | %80
	LD	(_io_bus_mode_and_timing), A

	LD	A, MEM_BUS_CYCLES | %80
	LD	(_mem_bus_mode_and_timing), A

	LD	A, MEM_BUS_CYCLES | %80
	LD	(_mem0_bus_mode_and_timing), A

	LD	A, MEM_BUS_CYCLES | %80
	LD	(_mem1_bus_mode_and_timing), A

	; Auto configure CS0's timing
	LD	A, 0
	LD	B, 17
	ld	hl, 70 ; 80ns
	ld	e, 1
	RST.L	%10

	; Auto configure CS1's timing
	LD	A, 0
	LD	B, 18
	ld	hl, 30 ; 80ns
	ld	e, 1
	RST.L	%10

	;XREF	_spike
	;call	_spike

IFDEF ZEXALL
	xref	Z80test
	call	Z80test
ENDIF

	LD	A, Z80_ADDR_MBASE		; set MBASE to $03
	LD	MB, A

	;XREF	z80_invoke
	;ld	iy, 0
	;call	z80_invoke

	JP.SIS	0				; transfer to external Memory under Z80 Compatible mode

	GLOBAL	remove_usb_tick_hook
	; remove the usb key/mouse inter handler hook
remove_usb_tick_hook:
	XREF 	_system_timer_isr
	LD	HL, _system_timer_isr
	XOR	A
	LD	(HL), A
	INC 	HL
	LD	(HL), A
	INC 	HL
	LD	(HL), A
	INC 	HL
	LD	(HL), A
	INC 	HL
	RET
