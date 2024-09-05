	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_ErasePages
; extern int8_t IFL_ErasePages(const uint32_t flash_addr, uint8_t number_of_pages);
_IFL_ErasePages:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)			; flash_addr low byte
	LD	H, (IX+5)			; flash_addr high byte
	LD	E, (IX+6)			; flash_addr high byte
	EZ80_CPY_EHL_TO_UHL

	LD	E, (IX+8)			; number_of_pages

	EZ80_IFL_ERASEPAGES

	POP	IX
	LD	L, A
	RET
