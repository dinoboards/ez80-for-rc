	include "../ez80-instr.inc"

	SECTION code_compiler

	PUBLIC	_IFL_Read
; extern void IFL_Read(uint8_t *pDst, uint32_t addr, uint8_t length);
_IFL_Read:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+6)			; source_addr low byte
	LD	H, (IX+7)			; source_addr high byte
	LD	E, (IX+8)			; source_addr upper byte
	EZ80_CPY_EHL_TO_UHL

	LD	E, (IX+4)			; pDst low byte
	LD	D, (IX+5)			; pDst high byte
	EZ80_CPY_MBDE_TO_UDE

	LDBCMM_LIL(0)
	LD	C, (IX+10)			; len low byte

	LDIR_LIL

	POP	IX
	RET
