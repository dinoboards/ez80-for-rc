
        INCLUDE "..\..\..\config.inc"
	INCLUDE "..\usb-constants.inc"

	SECTION CODE

	.assume adl=1

	XREF	_alt_read_index
	XREF	_reports

	GLOBAL	_usb_kyb_install_timer_tick
	XREF	_system_timer_isr
	XREF	_system_timer_isr_hook
	XREF	_usb_kyb_tick

_usb_kyb_install_timer_tick:
	LD	HL, (_system_timer_isr_hook+1)
	LD	(timer_isr_hook_next+1), HL
	LD	HL, timer_isr_hook
	LD	(_system_timer_isr_hook+1), HL
	RET

timer_isr_hook:
	PUSH	IX
	PUSH	IY
	PUSH	HL
	PUSH	DE
	PUSH	BC
	PUSH	AF
	CALL	_usb_kyb_tick
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	POP	IY
	POP	IX
	JP	timer_isr_hook_next

	SECTION DATA

timer_isr_hook_next:
	JP	_system_timer_isr

	SECTION CODE

	GLOBAL	_report_diff
	XREF	_report
	XREF	_previous

; uint8_t report_diff();
_report_diff:
	LD	DE, _report
	LD	HL, _previous
	LD	B, 8 				; //sizeof(keyboard_report_t)

.rptdf_loop:
	LD	A, (DE)
	INC	DE
	CP	(HL)
	INC	HL
	JR	NZ, .rptdf_diff
	DJNZ	.rptdf_loop

	XOR	A
	RET

.rptdf_diff:
	LD	A, 1
	RET

	GLOBAL	_usb_kyb_tick
	XREF	_in_critical_usb_section
	XREF	_ch_command
	XREF	_keyboard_config
	XREF	_usbdev_dat_in_trnsfer_0
	XREF	_keyboard_buf_put
	XREF	_keyboard_report_put

_usb_kyb_tick:
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

	LD	C, 8
	PUSH	BC
	LD	BC, _report
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

	LD	HL, _report
	LD	DE, _previous
	LD	BC, 8
	LDIR

	RET
