;
; Generated from source-doc/../../../../../ez80-clang/src/v99x8/vdp_cmd_pset.asm -- not to be modify directly
;
; 

	.assume	adl=1

	SECTION CODE

	include	"common.inc"

	SECTION CODE
	.assume	adl=1

	.global	_vdp_cmd_pset

; extern void vdp_cmd_pset(uint16_t x, uint16_t y, uint8_t colour, uint8_t operation);

	; x => iy + 3
	; y => iy + 6
	; colour => iy + 9
	; operation => iy + 12

_vdp_cmd_pset:
	ld	iy, 0
	add	iy, sp

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

	xor	a
	out	(bc), a					; N/A #R40

	xor	a
	out	(bc), a					; N/A #R41

	xor	a
	out	(bc), a					; N/A #R42

	xor	a
	out	(bc), a					; N/A #R43

	ld	a, (iy+9)				; load colour
	out	(bc), a					; into #R44

	xor	a					; Memory select - not configurable yet
	out	(bc), a					; into #R45

	ld	a, (iy+12)				; load operation
	or	CMD_PSET
	out	(bc), a					; into #R46

	RESTORE_IO_SPEED
	ret

