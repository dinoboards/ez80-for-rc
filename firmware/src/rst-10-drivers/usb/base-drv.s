

        INCLUDE "..\..\config.inc"
	SECTION CODE
	.assume adl=1

	XREF	_ch_get_status
	XREF	_delay
	XREF	_ch_command

	INCLUDE "usb-constants.inc"
	INCLUDE "base-drv\ch376asm.inc"

; HL -> timeout
; returns
; L -> error code

; ---------------------------------
; Function ch_wait_int_and_get_status
; ---------------------------------
	GLOBAL	_ch_wait_int_and_get_status
_ch_wait_int_and_get_status:
	ld	iy, 0
	add	iy, sp
	xor	a
	sbc	hl, hl
	ld	l, (iy+3)
	ld	h, (iy+4)

try_again:
	ld	bc, DELAY_FACTOR

keep_waiting:
	ld	a, %FF
	in	a, (_CH376_COMMAND_PORT & %FF)
	rlca
	jp	nc, _ch_get_status

	dec	bc
	ld	a, b
	or	c
	jr	nz, keep_waiting

	dec	hl
	ld	a, h
	or	l
	jr	nz, try_again

	call	_delay
	ld	a, %FF
	in	a, (_CH376_COMMAND_PORT & %FF)
	bit	4, a			; _CH376_COMMAND_PORT & PARA_STATE_BUSY

	ld	a, %0C 			; USB_ERR_CH376_BLOCKED;
	ret	nz

	ld	a, %0D 			; USB_ERR_CH376_TIMEOUT
	ret

; uint8_t ch_read_data(uint8_t *buffer);
	GLOBAL	_ch_read_data
_ch_read_data:
	CH_CMND	CH_CMD_RD_USB_DATA0

	ld	iy, 0
	add	iy, sp
	ld	hl, (iy+3)

	ld	bc, _CH376_DATA_PORT
	in	a, (bc)

	or	a
	ret	z

	ld	e, a
	push	af
read_block:
	in	a, (bc)
	ld	(hl), a
	inc	hl
	dec	e
	jr	nz, read_block

	pop	af
	ret

;const uint8_t *ch_write_data(const uint8_t *buffer, uint8_t length)
	GLOBAL	_ch_write_data
_ch_write_data:
	CH_CMND	CH_CMD_WR_HOST_DATA

	ld	iy, 0
	add	iy, sp
	ld	hl, (iy+3)
	ld	a, (iy+6)

	ld	bc, _CH376_DATA_PORT
; _CH376_DATA_PORT = length;
	out	(bc), a

	or	a
	ret	z

	ld	d, a
write_block:
	ld	a, (hl)
	out	(bc), a
	inc	hl
	dec	d
	jr	nz, write_block

	ret
