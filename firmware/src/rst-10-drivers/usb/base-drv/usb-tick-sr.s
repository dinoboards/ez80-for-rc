        INCLUDE "..\..\..\config.inc"
	INCLUDE "..\usb-constants.inc"

	SECTION CODE

	.assume adl=1

	GLOBAL	_usb_tick_sr
	XREF	_in_critical_usb_section
	XREF	_keyboard_config
	XREF	_ch_command
	XREF	_kyb_report
	XREF	_usbdev_dat_in_trnsfer_0
	XREF	_report_diff
	XREF	_keyboard_report_put
	XREF	_keyboard_buf_put
	XREF	_kyb_previous

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

	; return if keyboard_config == NULL
	LD	HL, (_keyboard_config)

	ADD	HL, DE
	OR	A, A
	SBC	HL, DE
	RET	Z				; return if keyboard_config == NULL

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

	LD	C, 8
	PUSH	BC
	LD	BC, _kyb_report
	LD	HL, (_keyboard_config)
	PUSH	BC
	PUSH	HL
	CALL	_usbdev_dat_in_trnsfer_0
	POP	BC
	POP	BC
	POP	BC
	PUSH	AF				; save result of _usbdev_dat_in_trnsfer_0

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

	POP	AF				; restore result of _usbdev_dat_in_trnsfer_0
	OR	A
	RET	NZ				; _usbdev_dat_in_trnsfer_0 returned NZ, so no usb report available

	CALL	_report_diff
	OR	A
	RET	Z				; return if no difference

	CALL	_keyboard_report_put

	LD	B, 6
.tickloop:
	LD	C, B
	DEC	C
	PUSH	BC
	CALL	_keyboard_buf_put
	POP	BC
	DJNZ	.tickloop

	LD	HL, _kyb_report
	LD	DE, _kyb_previous
	LD	BC, 8
	LDIR

	RET
