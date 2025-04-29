        INCLUDE "..\..\..\config.inc"
	INCLUDE "..\usb-constants.inc"
	INCLUDE "..\kyb-drv\kyb-interrupt.inc"
	INCLUDE "..\mse-drv\mse-interrupt.inc"

	SECTION CODE

	.assume adl=1

	GLOBAL	_usb_tick_sr
	XREF	_in_critical_usb_section
	XREF	_ch_command

	SEGMENT	INTERNAL_RAM_ROM

_usb_tick_sr:
	PUSH	IX
	PUSH	IY
	PUSH	HL
	PUSH	DE
	PUSH	BC
	PUSH	AF
	CALL	_usb_tick
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	POP	IY
	POP	IX
	RET

_usb_tick:
	LD	A, (_in_critical_usb_section)
	OR	A
	RET	NZ

	; ch_configure_nak_retry_disable();
	LD	L, CH_CMD_WRITE_VAR8
	PUSH	HL
	CALL	_ch_command
	POP	AF
	LD	A, CH_VAR_RETRY_TIMES
	LD	BC, _CH376_DATA_PORT
	OUT	(BC), A
	LD	A, CH_NAK_RETRY_DONT << 6 | %1F
	LD	BC, _CH376_DATA_PORT
	OUT	(BC), A

	KYB_DRIVER_INT_RETRIEVE
	PUSH	AF				; save result of KYB_DRIVER_INT_RETRIEVE(_usbdev_dat_in_trnsfer_0)

	MSE_DRIVER_INT_RETRIEVE
	PUSH	AF				; save result of MSE_DRIVER_INT_RETRIEVE(_usbdev_dat_in_trnsfer_0)

	; ch_configure_nak_retry_disable
	LD	E, CH_CMD_WRITE_VAR8
	PUSH	DE
	CALL	_ch_command
	POP	AF
	LD	A, CH_VAR_RETRY_TIMES
	LD	BC, _CH376_DATA_PORT
	OUT	(BC), A
	LD	A, CH_NAK_RETRY_3S << 6 | %1F
	LD	BC, _CH376_DATA_PORT
	OUT	(BC), A

	POP	AF				; restore result of MSE_DRIVER_INT_RETRIEVE(_usbdev_dat_in_trnsfer_0)
	MSE_DRIVER_INT_INJECT

	POP	AF				; restore result of KYB_DRIVER_INT_RETRIEVE(_usbdev_dat_in_trnsfer_0)
	KYB_DRIVER_INT_INJECT

	RET
