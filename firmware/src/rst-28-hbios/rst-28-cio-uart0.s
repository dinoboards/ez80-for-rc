	include "../config.inc"

	SECTION CODE

	.ASSUME ADL=1

	XREF	firmware_rst_10_hook

DISP_UART	EQU	3

	PUBLIC	_uart0_cio
_uart0_cio:
	; passthrough to firmware uart handler
	LD	A, DISP_UART
	JP	firmware_rst_10_hook
