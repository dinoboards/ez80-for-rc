	include	"..\..\..\config.inc"
	include	"..\usb-constants.inc"
	include	"..\kyb-drv\kyb-interrupt.inc"
	include	"..\mse-drv\mse-interrupt.inc"
	include	"..\base-drv\ch376asm.inc"

	section	CODE

	.assume	adl=1

	global	_usb_tick_sr
	XREF	_in_critical_usb_section
	XREF	_ch_command
	XREF	usb_tick_toggle

	segment	INTERNAL_RAM_ROM

_usb_tick_sr:
	push	af
	call	_usb_tick1
	pop	af
	ret

_usb_tick1:
	ld	a, (_in_critical_usb_section)
	or	a
	ret	nz

	ld	a, (usb_tick_toggle)
	xor	a, %FF
	ld	(usb_tick_toggle), a
	ret	z

	push	ix
	push	iy
	push	hl
	push	de
	push	bc
	call	_usb_tick2
	pop	bc
	pop	de
	pop	hl
	pop	iy
	pop	ix
	ret

_usb_tick2:
	; ch_configure_nak_retry_disable();
	CH_CMND	CH_CMD_WRITE_VAR8
	ld	a, CH_VAR_RETRY_TIMES
	ld	bc, _CH376_DATA_PORT
	out	(BC), a
	ld	a, CH_NAK_RETRY_DONT<<6|%1F
	out	(BC), a

	KYB_DRIVER_INT_RETRIEVE
	push	af				; save result of KYB_DRIVER_INT_RETRIEVE(_usbdev_dat_in_trnsfer_0)

	MSE_DRIVER_INT_RETRIEVE
	push	af				; save result of MSE_DRIVER_INT_RETRIEVE(_usbdev_dat_in_trnsfer_0)

	; ch_configure_nak_retry_disable
	CH_CMND	CH_CMD_WRITE_VAR8
	ld	a, CH_VAR_RETRY_TIMES
	ld	bc, _CH376_DATA_PORT
	out	(BC), a
	ld	a, CH_NAK_RETRY_3S<<6|%1F
	out	(BC), a

	pop	af				; restore result of MSE_DRIVER_INT_RETRIEVE(_usbdev_dat_in_trnsfer_0)
	MSE_DRIVER_INT_INJECT

	pop	af				; restore result of KYB_DRIVER_INT_RETRIEVE(_usbdev_dat_in_trnsfer_0)
	KYB_DRIVER_INT_INJECT

	ret
