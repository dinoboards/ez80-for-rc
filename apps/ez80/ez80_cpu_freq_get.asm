	include "ez80-instr.inc"

	.assume	adl=1

	section	.text,"ax",@progbits
	.global	_ez80_cpu_freq_get

; extern uint32_t ez80_cpu_freq_get(void);
_ez80_cpu_freq_get:
	EZ80_UTIL_GET_CPU_FQ
	PUSH	BC
	POP	HL
	LD	E, A
	RET

