	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_ProgramInfoPage
; extern int8_t IFL_ProgramInfoPage(uint8_t flash_addr, uint8_t *source_addr, uint8_t length);
_IFL_ProgramInfoPage:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)			; flash_addr
	LD	E, (IX+5)			; source_addr low byte
	LD	D, (IX+6)			; source_addr high byte
	EZ80_UTIL_MBDE_TO_UDE

	LD	C, (IX+7)			; len

	EZ80_IFL_PROGRAMINFOPAGE

	POP	IX
	LD	L, A
	RET


