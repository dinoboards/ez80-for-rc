	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_WriteInfoByte
; extern int8_t IFL_WriteInfoByte(const uint8_t addr, const uint8_t data);
_IFL_WriteInfoByte:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)			; addr within an info page
	LD	H, (IX+5)			; data

	EZ80_IFL_WRITEINFOBYTE

	POP	IX
	LD	L, A
	RET
