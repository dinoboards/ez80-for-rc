	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_IsAddrValid
; extern uint8_t IFL_IsAddrValid(const uint32_t flash_addr, const uint16_t length)
_IFL_IsAddrValid:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)			; flash_addr low byte
	LD	H, (IX+5)			; flash_addr high byte
	LD	E, (IX+6)			; len low byte
	EZ80_CPY_EHL_TO_UHL

	LD	E, (IX+8)			; len low byte
	LD	D, (IX+9)			; len high byte
	EZ80_IFL_ISADDRVALID

	POP	IX
	LD	L, A
	RET
