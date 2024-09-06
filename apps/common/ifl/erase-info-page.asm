	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_EraseInfoPage
; extern int8_t IFL_EraseInfoPage(const uint8_t addr);
_IFL_EraseInfoPage:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)			; addr within an info page

	EZ80_IFL_ERASEINFOPAGE

	POP	IX
	LD	L, A
	RET
