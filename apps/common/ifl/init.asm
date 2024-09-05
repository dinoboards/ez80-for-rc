	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_Init
; extern void IFL_Init(void);
_IFL_Init:
	EZ80_IFL_INIT
	RET
