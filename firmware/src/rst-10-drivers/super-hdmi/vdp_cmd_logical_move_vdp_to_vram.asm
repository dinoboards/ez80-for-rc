;
; Generated from source-doc/../../../../../ez80-clang/src/v99x8/vdp_cmd_logical_move_vdp_to_vram.asm -- not to be modify directly
;
; 

	.assume	adl=1

	SECTION CODE

	include	"common.inc"

	SECTION CODE
	.assume	adl=1

	.global	_vdp_cmd_logical_move_vdp_to_vram

; extern void vdp_cmd_logical_move_vdp_to_vram(
;     uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t colour, uint8_t direction, uint8_t operation);

	; x => iy + 3
	; y => iy + 6
	; width => iy + 9
	; height => iy + 12
	; colour => iy + 15
	; direction => iy + 18
	; operation => iy + 21

_vdp_cmd_logical_move_vdp_to_vram:
	ld	iy, 0
	add	iy, sp

	DI_AND_SAVE
	SET_SLOW_IO_SPEED

	ld	bc, (_VDP_IO_ADDR)
	ld	a, 36					; submit 36, with auto increment
	out	(bc), a
	ld	a, %80|17				; to register 17
	out	(bc), a

	ld	bc, (_VDP_IO_REGS)

	ld	hl, (iy+3)				; load x
	out	(bc), l					; low byte into #R36
	out	(bc), h					; high byte into #R37

	ld	hl, (iy+6)				; load y
	out	(bc), l					; low byte into #R38
	out	(bc), h					; high byte into #R39

	ld	hl, (iy+9)				; load width
	out	(bc), l					; low byte into #R40
	out	(bc), h					; high byte into #R41

	ld	hl, (iy+12)				; load height
	out	(bc), l					; low byte into #R42
	out	(bc), h					; high byte into #R43

	ld	a, (iy+15)				; load colour
	out	(bc), a					; into #R44

	ld	a, (iy+18)				; load direction
	out	(bc), a					; into #R45

	ld	a, (iy+21)				; load operation
	or	CMD_LMMV
	out	(bc), a					; into #R46

	RESTORE_IO_SPEED
	RESTORE_EI

	ret
