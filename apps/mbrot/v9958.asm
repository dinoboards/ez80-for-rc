
	.assume	adl = 1

	section	.text,"ax",@progbits

	.global	_outDat, _outCmd, _commandDrawLine, _waitForCommandCompletion
	.global	__fromX, __fromY, _longSide, _shortSide, __color, _dir, __operation


; V9958 REGISTERS
VDP_DATA:	equ	$FF98		; VDP data port (VRAM read/write)
VDP_ADDR:	equ	$FF99		; VDP address (write only)
VDP_STAT:	equ	$FF99		; VDP status (read only)
VDP_PALT:	equ	$FF9A		; VDP palette latch (write only)
VDP_REGS:	equ	$FF9B		; VDP register access (write only)


; void outDat(uint8_t b)
_outDat:
	LD	IY, 0
	ADD	IY, SP
	LD	L, (IY + 3)
	LD	BC, VDP_DATA
	OUT	(BC), L
	RET

; void outCmd(uint8_t b) __z88dk_fastcall
_outCmd:
	LD	IY, 0
	ADD	IY, SP
	LD	L, (IY + 3)
	LD	BC, VDP_ADDR
	RET

_commandDrawLine:
	DI
	; Set read register to 2 (status)
	LD	BC, VDP_ADDR
	LD	A, 2
	OUT	(BC), A
	LD	A, 0x80 | 15
	OUT	(BC), A

	; WAIT FOR ANY PREVIOUS COMMAND TO COMPLETE
_commandDrawLineReady:
	IN	A, (BC)
	RRCA
	JR	C, _commandDrawLineReady

	; SET INDIRECT REGISTER TO 36
	LD	A, 36
	OUT	(BC), A
	LD	A, 0x80 | 17
	OUT	(BC), A

	LD	HL, __fromX
	LD	DE, VDP_REGS
	LD	BC, 11
	OTIRX

	LD	BC, VDP_ADDR
	XOR	A
	OUT	(BC), A
	LD	A, 0x80 | 15
	OUT	(BC), A

	EI
	RET

_waitForCommandCompletion:
	DI
	; Set read register to 2
	LD	BC, VDP_ADDR

	LD	A, 2
	OUT	(BC), A
	LD	A, 0x80 | 15
	OUT	(BC), A

_waitForCommandCompletionLoop:
	IN	A, (BC)
	RRCA
	JR	C, _waitForCommandCompletionLoop

	XOR	A
	OUT	(BC), A
	LD	A, 0x80 | 15
	OUT	(BC), A

	EI
	RET

	section	.data,"aw",@progbits

__fromX:	DW	0
__fromY:	DW	0
_longSide:	DW	0
_shortSide:	DW	0
__color:	DB	0
_dir:		DB	0
__operation:	DB	0

