	XREF	_mouse_config
	XREF	_mse_report
	XREF	_usbdev_dat_in_trnsfer_0
	XREF	_mouse_report_diff
	XREF	_mouse_inject
	XREF	_mouse_buf_put
	XREF	_mse_previous

MSE_DRIVER_INT_RETRIEVE:	MACRO
	LD	HL, (_mouse_config)

	ADD	HL, DE
	OR	A, A
	SBC	HL, DE
	JR	NZ, $$skip1			; return if mouse_config == NULL

	XOR	A
	CPL
	JR	$$skip2				; unset Z flag

$$skip1:
	LD	C, 8
	PUSH	BC
	LD	BC, _mse_report
	PUSH	BC
	PUSH	HL
	CALL	_usbdev_dat_in_trnsfer_0
	POP	BC
	POP	BC
	POP	BC
$$skip2:
	ENDM

MSE_DRIVER_INT_INJECT:	MACRO
	OR	A
	JR	NZ, $$skip1				; _usbdev_dat_in_trnsfer_0 returned NZ, so no usb report available

	CALL	_mouse_inject

$$skip1:
	ENDM
