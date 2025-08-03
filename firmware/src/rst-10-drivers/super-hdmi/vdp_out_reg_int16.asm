;
; Generated from source-doc/../../../../../ez80-clang/src/v99x8/vdp_out_reg_int16.asm -- not to be modify directly
;
; 

	.assume	adl=1

	SECTION CODE

	include	"common.inc"

	.global	_vdp_out_reg_int16
; void vdp_out_reg_int16(uint16_t b)
_vdp_out_reg_int16:
	DI_AND_SAVE
	SET_SLOW_IO_SPEED

	ld	iy, 0
	add	iy, sp
	ld	hl, (IY+3)
	ld	bc, (_VDP_IO_REGS)
	out	(bc), l
	xor	a
	out	(bc), h

	RESTORE_IO_SPEED
	RESTORE_EI

	ret
