	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_GetPage
; extern uint8_t IFL_GetPage(const void *flash_addr);
_IFL_GetPage:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)			; flash_addr low byte
	LD	H, (IX+5)			; flash_addr high byte
	EZ80_UTIL_MBHL_TO_UHL

	EZ80_IFL_GETPAGE

	POP	IX
	LD	L, A
	RET
