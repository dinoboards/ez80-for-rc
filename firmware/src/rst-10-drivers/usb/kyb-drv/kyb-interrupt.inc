	XREF	_keyboard_config
	XREF	_kyb_report
	XREF	_usbdev_dat_in_trnsfer_0
	XREF	_report_diff
	XREF	_keyboard_report_put
	XREF	_keyboard_buf_put
	XREF	_kyb_previous

KYB_DRIVER_INT_RETRIEVE:	MACRO
	LD	HL, (_keyboard_config)

	ADD	HL, DE
	OR	A, A
	SBC	HL, DE
	JR	NZ, $$skip1			; return if keyboard_config == NULL

	XOR	A
	CPL
	JR	$$skip2				; unset Z flag

$$skip1:
	LD	C, 8
	PUSH	BC
	LD	BC, _kyb_report
	PUSH	BC
	PUSH	HL
	CALL	_usbdev_dat_in_trnsfer_0
	POP	BC
	POP	BC
	POP	BC
$$skip2:
	ENDM

KYB_DRIVER_INT_INJECT:	MACRO
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

	ENDM
