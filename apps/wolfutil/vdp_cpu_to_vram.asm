	.assume	adl=1
	section	.text._vdp_cpu_to_vram, "ax", @progbits
	.global	_vdp_cpu_to_vram

; fast hdmi version

; void vdp_cpu_to_vram(const uint8_t* const source, uint24_t vdp_address, uint16_t length)

VDP_DATA:	equ	$FF98		; VDP data port (VRAM read/write)
VDP_ADDR:	equ	$FF99		; VDP address (write only)

_vdp_cpu_to_vram:
	ld	iy, 0
	add	iy, sp
	; iy + 3 -> source
	; iy + 6 -> vdp_address
	; iy + 9 -> length

	; Write to VDP_ADDR:
	;  0000 0 <A16..A14> 	of vdp_address
	;  1000 1110 		select register 14
	;  <A7..A0> 		of vdp_address
	;  01 <A13..A8> 	of vdp_address to enable write mode

	ld	a, (iy+8) 		; vdp_address bits 16..23

	and	%00000001		; extract only bit 16
	rlca				; move 'B16' to B1
	rlca				; move 'B16' to B2
	ld	b, a			; save


	ld	a, (iy+7) 		; vdp_address bits 8..15
	and	%11000000		; extract bits 15 and 14
	rlca				; move 'B15' to B0, 'B14' to B7
	rlca				; move 'B15' to B1, 'B14' to B0
	or	b			; merge with B16


	ld	bc, VDP_ADDR
	out	(BC), a			; value for reg 14 (B16..B14)
	ld	a, $80+14		; VDP register 14
	out	(BC), a

	ld	a, (iy+6)		; vdp_address bits 0..7
	out	(BC), a			; submit bits 0 to 7

	ld	a, (iy+7)		; vdp_address bits 8..15
	and	%00111111		; extract bits 8..13
	or	%01000000		; enable write mode
	out	(BC), a			; submit bits 8 to 13

	ld	de, (iy+9)		; length
	ld	hl, (iy+3)		; source
	ld	bc, VDP_DATA

loop:
	ld	a, (hl)
	inc	hl
	out	(BC), a
	dec	de
	ld	a, e		; warning only 16bit counter
	or	d
	jr	nz, loop

	ret

