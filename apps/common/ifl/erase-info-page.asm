	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_EraseInfoPage
; extern int8_t IFL_EraseInfoPage(void);
_IFL_EraseInfoPage:
	EZ80_IFL_ERASEINFOPAGE
	LD	L, A
	RET
