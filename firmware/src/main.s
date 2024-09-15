	INCLUDE	"config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	_main
	XREF	_uart0_init
	XREF	_bank_init_z2
	XREF	_rx_buffer_init
	XREF	_init_clocks
	XREF	__c_startup
	XREF	_attempt_alt_firmware
	XREF	_IFL_Init
	XREF	_configure_onchip_mem

_main:
	CALL	__c_startup
	CALL	_init_clocks
	CALL	_bank_init_z2
	CALL	_rx_buffer_init
	CALL	_uart0_init

IFNDEF RC2014_ALT_FIRMWARE
	CALL	_IFL_Init
	CALL	_attempt_alt_firmware			; may not return if alt-bios present and ok to boot with
ENDIF

	;XREF	_spike
	;call	_spike

	LD	A, Z80_ADDR_MBASE		; set MBASE to $03
	LD	MB, A
	JP.SIS	0				; transfer to external Memory under Z80 Compatible mode

	SEGMENT BSS

_tmp:
	DS	3
