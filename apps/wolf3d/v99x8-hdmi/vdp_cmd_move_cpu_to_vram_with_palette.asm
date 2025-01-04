

	.assume	adl=1

	section	.text_on_chip, "ax", @progbits

	include	"v99x8-hdmi/common.inc"

	section	.text_on_chip, "ax", @progbits
	.assume	adl = 1

	.global	_vdp_cmd_move_cpu_to_vram_with_palette

; extern void vdp_cmd_move_cpu_to_vram_with_palette(
;    const uint8_t *source, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t direction, uint24_t length, byte palette[256]);

	; source => iy + 3
	; x => iy + 6
	; y => iy + 9
	; width => iy + 12
	; height => iy + 15
	; direction => iy + 18
	; length => iy + 21
	; palette => iy + 24

_vdp_cmd_move_cpu_to_vram_with_palette:
	ld	iy, 0
	add	iy, sp

	exx
	ld	de, (iy+24)
	exx

	ld	bc, VDP_ADDR				;
	ld	a, 36					; submit 36 with auto increment
	out	(bc), a
	DELAY_VDP
	ld	a, 0x80 | 17				; to register 17
	out	(bc), a

	ld	bc, VDP_REGS

	ld	hl, (iy + 6)				; load x
	DELAY_VDP
	out	(bc), l					; low byte into #R36
	DELAY_VDP
	out	(bc), h					; high byte into #R37

	ld	hl, (iy + 9)				; load y
	DELAY_VDP
	out	(bc), l					; low byte into #R38
	DELAY_VDP
	out	(bc), h					; high byte into #R39

	ld	hl, (iy + 12)				; load width
	DELAY_VDP
	out	(bc), l					; low byte into #R40
	DELAY_VDP
	out	(bc), h					; high byte into #R41

	ld	hl, (iy + 15)				; load height
	DELAY_VDP
	out	(bc), l					; low byte into #R42
	DELAY_VDP
	out	(bc), h					; high byte into #R43

	DELAY_VDP
	ld	hl, (iy + 3)				; load source
	ld	a, (hl)					; load first byte

	; apply the palette
	exx				; 1
	or	a			; 1
	sbc	hl, hl			; 2
	ld	l, a			; 1
	add	hl, de			; 2
	ld	a, (hl)			; 2
	exx				; 1

	inc	hl
	out	(bc), a					; into #R44

	DELAY_VDP
	ld	a, (iy + 18)				; load direction
	out	(bc), a					; into #R45

	DELAY_VDP
	ld	a, CMD_HMMC				; submit command
	out	(bc), a

	ld	de, (iy + 21)				; load length

	ld	bc, VDP_ADDR				;
	ld	a, 0x80 | 44				; submit 44 without auto increment
	out	(bc), a
	DELAY_VDP
	ld	a, 0x80 | 17				; to register 17
	out	(bc), a

	ld	bc, VDP_REGS

loop:
	DELAY_VDP
	ld	a, (hl)					; load next byte

	; apply the palette
	exx				; 1
	or	a			; 1
	sbc	hl, hl			; 2
	ld	l, a			; 1
	add	hl, de			; 2
	ld	a, (hl)			; 2
	exx				; 1

	inc	hl
	out	(bc), a					; submit to VDP

	dec	de
	ld	a, e
	or	d
	jr	nz, loop

	ret
