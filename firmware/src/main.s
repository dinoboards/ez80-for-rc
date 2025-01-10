	INCLUDE	"config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	_main
	XREF	_uart0_init
	XREF	_rx_buffer_init
	XREF	_init_clocks
	XREF	__c_startup
	XREF	_configure_onchip_mem

	XREF	_io_bus_mode_and_timing
	XREF	_mem_bus_mode_and_timing
	XREF	_mem0_bus_mode_and_timing

_main:
	CALL	__c_startup
	CALL	_init_clocks
	CALL	_rx_buffer_init
	CALL	_uart0_init


	LD	A, IO_BUS_CYCLES | %80
	LD	(_io_bus_mode_and_timing), A

	LD	A, MEM_BUS_CYCLES | %80
	LD	(_mem_bus_mode_and_timing), A

	LD	A, MEM_BUS_CYCLES | %80
	LD	(_mem0_bus_mode_and_timing), A

	;XREF	_spike
	;call	_spike

	LD	A, Z80_ADDR_MBASE		; set MBASE to $03
	LD	MB, A
	JP.SIS	0				; transfer to external Memory under Z80 Compatible mode

	SEGMENT BSS

_tmp:
	DS	3
