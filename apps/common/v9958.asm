
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

	.global	_outDat
; void outDat(uint8_t b)
_outDat:
	LD	IY, 0
	ADD	IY, SP
	LD	A, (IY + 3)
	LD	BC, VDP_DATA
	OUT	(BC), A
	RET

	.global	_outCmd
; void outCmd(uint8_t b)
_outCmd:
	LD	IY, 0
	ADD	IY, SP
	LD	A, (IY + 3)
	LD	BC, VDP_ADDR
	OUT	(BC), A
	RET

	.global	_commandDrawLine
_commandDrawLine:
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
_commandDrawLineReady:
	IN	A, (BC)
	RRCA
	JR	C, _commandDrawLineReady

	; SET INDIRECT REGISTER TO 36
	LD	A, 36
	DELAY_1_7US
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 17				; measured on CPU running @25Mhz
	OUT	(BC), A

	LD	HL, __fromX
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

	.global _waitForCommandCompletion
_waitForCommandCompletion:
	DI
	; Set read register to 2
	LD	BC, VDP_ADDR

	LD	A, 2
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 15				; measured on CPU running @25Mhz
	OUT	(BC), A

	DELAY_1_7US

_waitForCommandCompletionLoop:
	IN	A, (BC)
	RRCA
	JR	C, _waitForCommandCompletionLoop

	XOR	A
	DELAY_1_7US
	OUT	(BC), A
	DELAY_1_7US					; DELAY and LD provde the ~2us required delay
	LD	A, 0x80 | 15				; measured on CPU running @25Mhz
	OUT	(BC), A

	EI
	RET


	.global	_outPal
; void outPal(uint8_t b)
_outPal:
	LD	IY, 0
	ADD	IY, SP
	LD	A, (IY + 3)
	LD	BC, VDP_PALT
	OUT	(BC), A
	RET

	.global	_outRegIndByte
; void outRegIndByte(uint8_t b)
_outRegIndByte:
	LD	IY, 0
	ADD	IY, SP
	LD	A, (IY + 3)
	LD	BC, VDP_REGS
	OUT	(BC), A
	RET

	.global	_outRegIndInt
; void outRegIndInt(uint16_t b)
_outRegIndInt:
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

	.global	__writeRegister
; void __writeRegister(uint16_t rd)
__writeRegister:
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

	.global	__fromX, __fromY, __color, __operation, _longSide, _shortSide, _dir

__fromX:	DW	0
__fromY:	DW	0
_longSide:	DW	0
_shortSide:	DW	0
__color:	DB	0
_dir:		DB	0
__operation:	DB	0

