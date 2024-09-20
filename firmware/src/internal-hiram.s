	include ".\config.inc"

	DEFINE	INTERNAL_HIRAM_ROM, SPACE = ROM
	SEGMENT	INTERNAL_HIRAM_ROM

	DEFINE	INTERNAL_HIRAM_RAM, SPACE = RAM, ORG=%02FF00
	SEGMENT	INTERNAL_HIRAM_RAM

	.assume adl=1

	SEGMENT	INTERNAL_HIRAM_ROM

	PUBLIC	FIRMWARE_RAM_RESERVED
FIRMWARE_RAM_RESERVED:
_firmware_ram_reseved:

	XREF	_rst_08_functions
	PUBLIC	check_alt_firmware_rst_08
check_alt_firmware_rst_08:
	JP	_rst_08_functions

	XREF	_rst_10_functions
	PUBLIC	check_alt_firmware_rst_10
check_alt_firmware_rst_10:
	JP	_rst_10_functions

	XREF	_system_timer_isr
	PUBLIC	_system_timer_isr_hook
_system_timer_isr_hook:
	JP	_system_timer_isr

	XREF	_marshall_isr
	PUBLIC	_marshall_isr_hook
_marshall_isr_hook:
	JP	_marshall_isr

	XREF	_uart0_receive_isr
	PUBLIC	_uart0_receive_isr_hook
_uart0_receive_isr_hook:
	JP	_uart0_receive_isr

	PUBLIC	_default_mi_handler_hook
_default_mi_handler_hook:				; if alt-firmware wants to handle a default interrupt
	POP	AF					; it can change this to be a 'jump' to its own handler
	EI						; a will indicate the index of the interrupt
	RETI.L

	DB	0, 0, 0, 0

; EZ80_DELAY (RST %18)
; Wait 5 to 6 us (approx) for the eZ80
; indepedent of the clock speed
;
; Input
;  None
;
; Output
;  None
;
	PUBLIC	check_alt_firmware_rst_18
check_alt_firmware_rst_18:
	; //timer 0 has reload value that will equate to about 5-6us
; 	; so set it off, and wait.

	CRITICAL_BEGIN
	LD	A, TMR_ENABLED | TMR_SINGLE | TMR_RST_EN | TMR_CLK_DIV_4
	OUT0	(TMR0_CTL), A

wait:
	IN0	A, (TMR0_DR_L)
	JR	NZ, wait
	CRITICAL_END
	RET.L


_reserved:
	DB	0, 0, 0, 0, 0, 0, 0, 0
	DB	0, 0, 0, 0, 0, 0, 0, 0


