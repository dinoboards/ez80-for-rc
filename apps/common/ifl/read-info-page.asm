	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_ReadInfoPage
; extern int8_t IFL_ReadInfoPage(uint8_t *destination_addr, uint8_t flash_addr, uint8_t length);
_IFL_ReadInfoPage:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)			; destination_addr low byte
	LD	H, (IX+5)			; destination_addr high byte
	EZ80_CPY_MBHL_TO_UHL

	LD	E, (IX+6)			; flash_addr
	LD	C, (IX+7)			; len

	EZ80_IFL_READINFOPAGE

	POP	IX
	LD	L, A
	RET

