
	.assume	adl = 1

	section	.text,"ax",@progbits


; V9958 REGISTERS
VDP_DATA:	equ	$FF98		; VDP data port (VRAM read/write)
VDP_ADDR:	equ	$FF99		; VDP address (write only)
VDP_STAT:	equ	$FF99		; VDP status (read only)
VDP_PALT:	equ	$FF9A		; VDP palette latch (write only)
VDP_REGS:	equ	$FF9B		; VDP register access (write only)


.macro	DELAY_1_7US  ; approx 1.7 us @25Mhz CPU
	PUSH	AF
	POP	AF
	PUSH	AF
	POP	AF
	NOP
	NOP
.endm

	.global	_vmd_cmd_draw_line
_vmd_cmd_draw_line:
	DI
	; Set read register to 2 (status)
	LD	BC, VDP_ADDR
	LD	A, 2
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 15				; measured on CPU running @25Mhz
	OUT	(BC), A

	DELAY_1_7US

	; WAIT FOR ANY PREVIOUS COMMAND TO COMPLETE
_vmd_cmd_draw_line_wait:
	IN	A, (BC)
	RRCA
	JR	C, _vmd_cmd_draw_line_wait

	; SET INDIRECT REGISTER TO 36
	LD	A, 36
	DELAY_1_7US
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 17				; measured on CPU running @25Mhz
	OUT	(BC), A

	LD	HL, _vdp_cmdp_from_x
	LD	BC, VDP_REGS
	LD	A, 11
.outs:							; loop calibrated to have appro 2us
	PUSH	AF					; between rights
	POP	AF
	NOP						; spec seems to indicate we should have a period
							; of 8us after the 2nd byte is written
	LD	E, (HL)					; but we seem to get a way with 2us just fine???
	OUT	(BC), E
	INC	HL
	DEC	A
	JR	NZ, .outs

	LD	BC, VDP_ADDR
	XOR	A
	DELAY_1_7US
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 15				; measured on CPU running @25Mhz
	OUT	(BC), A

	EI
	RET

	.global _vdp_cmd_wait_completion
_vdp_cmd_wait_completion:
	DI
	; Set read register to 2
	LD	BC, VDP_ADDR

	LD	A, 2
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 15				; measured on CPU running @25Mhz
	OUT	(BC), A

	DELAY_1_7US

_vdp_cmd_wait_completion_wait:
	IN	A, (BC)
	RRCA
	JR	C, _vdp_cmd_wait_completion_wait

	XOR	A
	DELAY_1_7US
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 15				; measured on CPU running @25Mhz
	OUT	(BC), A

	EI
	RET

	.global	_vdp_out_reg_int16
; void vdp_out_reg_int16(uint16_t b)
_vdp_out_reg_int16:
	LD	IY, 0
	ADD	IY, SP
	LD	HL, (IY + 3)
	LD	BC, VDP_REGS
	OUT	(BC), L
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0					; measured on CPU running @25Mhz
	OUT	(BC), H
	RET

	.global	inDat
; uint8_t inDat(void)
_inDat:
	LD	HL, 0
	LD	BC, VDP_DATA
	IN	L, (BC)
	RET

	.global	_readStatus
; uint8_t readStatus(uint8_t r)
_readStatus:
	LD	IY, 0
	ADD	IY, SP
	LD	L, (IY + 3)
	LD	BC, VDP_ADDR

	; SET READ REGISTER TO 15
	OUT	(BC), L
	DELAY_1_7US
	LD	A, 0x80 | 15
	OUT	(BC), A

	LD	HL, 0
	IN	L, (BC)					; READ STATUS

	DELAY_1_7US

	; RESTORE READ REGISTER TO DEFAULT OF 0
	XOR	A
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 15				; measured on CPU running @25Mhz
	OUT	(BC), A
	RET

	.global	__vdp_reg_write
; void __vdp_reg_write(uint16_t rd)
__vdp_reg_write:
	LD	IY, 0
	ADD	IY, SP
	LD	HL, (IY + 3)
	LD	BC, VDP_ADDR
	OUT	(BC), L
	LD	A, 0x80
	OR	H
	DELAY_1_7US
	OUT	(BC), A
	RET


	section	.data,"aw",@progbits

	.global	_vdp_cmdp_from_x, _vdp_cmdp_from_y, _vdp_cmdp_color, _vdp_cmdp_operation, _vdp_cmdp_long_side, _vdp_cmdp_short_side, _vdp_cmdp_dir

_vdp_cmdp_from_x:	DW	0
_vdp_cmdp_from_y:	DW	0
_vdp_cmdp_long_side:	DW	0
_vdp_cmdp_short_side:	DW	0
_vdp_cmdp_color:	DB	0
_vdp_cmdp_dir:		DB	0
_vdp_cmdp_operation:	DB	0

