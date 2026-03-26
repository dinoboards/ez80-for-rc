	include "../config.inc"
	include "../rst-28-constants.inc"

	SECTION CODE

	.ASSUME ADL=1

	XREF	firmware_rst_10_hook
	XREF	_vdu
	XREF	_uart0_cioout

	PUBLIC	_suphdmi_ciodevice
_suphdmi_ciodevice:
	LD	C, 040h					; terminal
	LD	D, CIODEV_SUPHDMI
	LD	E, 0
	LD	HL, 0
	RET.L

	PUBLIC	_suphdmi_cioout
_suphdmi_cioout:
	PUSH	BC
	PUSH	DE
	CALL	_vdu
	POP	DE
	POP	BC
	JP	_uart0_cioout
