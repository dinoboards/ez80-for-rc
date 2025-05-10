	.assume	adl=1

	section	.text, "ax", @progbits

	include	"v99x8-hdmi/common.inc"

	.global	_vdp_cpu_to_vram0_with_palette

; void vdp_cpu_to_vram0_with_palette(const uint8_t* const source, uint16_t length, byte palette[256])
;
; source => IY+3
; length => IY+6
; palette => IY+9

_vdp_cpu_to_vram0_with_palette:
	ld	iy, 0
	add	iy, sp

	SET_SLOW_IO_SPEED

	exx
	ld	de, (iy+9)
	exx

	; Write to VDP_ADDR:
	;  0000 0 <A16..A14> 	of vdp_address
	;  1000 1110 		select register 14
	;  <A7..A0> 		of vdp_address
	;  01 <A13..A8> 	of vdp_address to enable write mode

	xor	a

	ld	bc, VDP_ADDR
	out	(bc), a			; value for reg 14 (B16..B14)
	ld	a, $80+14		; VDP register 14
	out	(bc), a

	xor	a
	out	(bc), a			; submit bits 0 to 7

	ld	a, %01000000		; enable write mode
	out	(bc), a			; submit bits 8 to 13

	ld	de, (iy+6)		; length
	ld	hl, (iy+3)		; source
	ld	bc, VDP_DATA

	SET_SLOW_MEM_SPEED
loop:
	ld	a, (hl)

	; apply the palette
	exx				; 1
	or	a			; 1
	sbc	hl, hl			; 2
	ld	l, a			; 1
	add	hl, de			; 2
	ld	a, (hl)			; 2
	exx				; 1

	inc	hl
	out	(bc), a
	dec	de
	ld	a, e			; warning only 16bit counter
	or	d
	jr	nz, loop

	RESTORE_MEM_SPEED
	RESTORE_IO_SPEED

	ret

