	include "../ez80-instr.inc"

	SECTION code_compiler

	EXTERN	_EZ80_CPY_AUBC_TO_DEHL

	PUBLIC	_ez80_cpu_freq_get
; extern uint32_t ez80_cpu_freq_get(void);
_ez80_cpu_freq_get:

	EZ80_UTIL_GET_CPU_FQ
	JP	_EZ80_CPY_AUBC_TO_DEHL
