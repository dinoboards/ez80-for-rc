	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_Program
; extern int8_t IFL_Program(uint32_t flash_addr, uint8_t *source_addr, const uint16_t length);
_IFL_Program:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)			; flash_addr low byte
	LD	H, (IX+5)			; flash_addr high byte
	LD	E, (IX+6)			; flash_addr high byte
	EZ80_UTIL_EHL_TO_UHL

	LD	E, (IX+8)			; source_addr low byte
	LD	D, (IX+9)			; source_addr high byte
	EZ80_UTIL_MBDE_TO_UDE

	LD	C, (IX+10)			; len low byte
	LD	B, (IX+11)			; len high byte

	EXX

	EZ80_IFL_PROGRAM

	POP	IX
	LD	L, A
	RET
