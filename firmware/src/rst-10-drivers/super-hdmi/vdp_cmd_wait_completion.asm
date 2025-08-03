;
; Generated from source-doc/../../../../../ez80-clang/src/v99x8/vdp_cmd_wait_completion.asm -- not to be modify directly
;
; 

	.assume	adl=1

	SECTION CODE

	include	"common.inc"

	.global	_vdp_cmd_wait_completion
_vdp_cmd_wait_completion:
	; Set read register to 2

	DI_AND_SAVE
	SET_SLOW_IO_SPEED

	ld	bc, (_VDP_IO_ADDR)

	ld	a, 2
	out	(bc), a					; DELAY and LD provde the ~2us required delay
	ld	a, %80|15				; measured on CPU running @25Mhz
	out	(bc), a


	ld	de, %100000
_vdp_cmd_wait_completion_wait:
	in	a, (BC)
	rrca
	jr	nc, .exit 				; _vdp_cmd_wait_completion_ready
	dec	de
	ld	hl, 0
	xor	a
	sbc	hl, de
	jr	nz, _vdp_cmd_wait_completion_wait

.exit:
	RESTORE_IO_SPEED
	RESTORE_EI
	ret
