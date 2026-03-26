	.assume	adl=1

	SECTION CODE

	include	"common.inc"

	.global	_vdp_cpu_to_vram


; void vdp_cpu_to_vram(const uint8_t* const source, uint24_t vdp_address, uint16_t length)

_vdp_cpu_to_vram:
	ld	iy, 0
	add	iy, sp

	DI_AND_SAVE
	SET_SLOW_IO_SPEED

	; SET STATUS REGISTER to #02
	ld	bc, VDP_IO_ADDR
	ld	a, 2
	out	(BC), a
	ld	a, %80|15
	out	(BC), a

.waitb4:
	in	a, (bc)
	bit	2, a
	jr	z, .waitb4


	; iy + 3 -> source
	; iy + 6 -> vdp_address
	; iy + 9 -> length

	; Write to VDP_ADDR:
	;  0000 0 <A16..A14> 	of vdp_address
	;  1000 1110 		select register 14
	;  <A7..A0> 		of vdp_address
	;  01 <A13..A8> 	of vdp_address to enable write mode

	ld	a, (iy+8) 		; vdp_address bits 16..23

	and	00001111b		; extract bit 16..19 (super supports up to 20bit address)
	rlca				; move 'B16' to B1
	rlca				; move 'B16' to B2
	ld	b, a			; save

	ld	a, (iy+7) 		; vdp_address bits 8..15
	and	011000000b		; extract bits 15 and 14
	rlca				; move 'B15' to B0, 'B14' to B7
	rlca				; move 'B15' to B1, 'B14' to B0
	or	b			; merge with B16 to B18

	ld	bc, VDP_IO_ADDR
	out	(BC), a			; value for reg 14 (B18..B14)
	ld	a, %80+14		; VDP register 14
	out	(BC), a

	ld	a, (iy+6)		; vdp_address bits 0..7
	out	(BC), a			; submit bits 0 to 7

	ld	a, (iy+7)		; vdp_address bits 8..15
	and	000111111b		; extract bits 8..13
	or	001000000b		; enable write mode
	out	(BC), a			; submit bits 8 to 13

	ld	de, (iy+9)		; length
	ld	hl, (iy+3)		; source
	ld      bc, VDP_IO_DATA

	exx
	ld	bc, VDP_IO_ADDR
	exx
loop:
	exx
.wait:
	in	a, (bc)
	bit	2, a
	jr	z, .wait

	exx

	ld	a, (hl)
	inc	hl
	out	(BC), a
	dec	de
	ld	a, e		; warning only 16bit counter
	or	d
	jr	nz, loop

	ld	bc, VDP_IO_ADDR
	xor	a
	out	(BC), a
	ld	a, %80|15
	out	(BC), a

	RESTORE_IO_SPEED
	RESTORE_EI

	ret

