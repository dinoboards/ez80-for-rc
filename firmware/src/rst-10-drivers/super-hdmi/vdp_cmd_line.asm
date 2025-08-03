;
; Generated from source-doc/../../../../../ez80-clang/src/v99x8/vdp_cmd_line.asm -- not to be modify directly
;
; 


	.assume	adl=1

	SECTION CODE

	include	"common.inc"

	SECTION CODE
	.assume	adl=1

	.global	_vdp_cmd_line


; extern void vdp_cmd_line(
;     uint16_t x, uint16_t y, uint16_t long_length, uint16_t short_length, uint8_t direction, uint8_t colour, uint8_t operation);
; x => iy + 3
; y => iy + 6
; long_length => iy+9
; short_length => iy+12
; direction => iy+15
; colour => iy+18
; operation => iy+21

_vdp_cmd_line:
	ld	iy, 0
	add	iy, sp

	SET_SLOW_IO_SPEED

	ld	bc, (_VDP_IO_ADDR)				;
	ld	a, 36					; submit 36 with auto increment
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

	ld	hl, (iy+9)				; load long_length
	out	(bc), l					; low byte into #R40
	out	(bc), h					; high byte into #R41

	ld	hl, (iy+12)				; load short_length
	out	(bc), l					; low byte into #R42
	out	(bc), h					; high byte into #R43

	ld	hl, (iy+18)				; load colour
	out	(bc), l					; low byte into #R44

	ld	a, (iy+15)				; load direction
	out	(bc), a					; low byte into #R45

	ld	a, (iy+21)				; load operation
	or	CMD_LINE				; mask in command
	out	(bc), a					; low byte into #R46

	RESTORE_IO_SPEED

	ret
