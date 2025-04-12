
	.assume	adl=1
	section	.text, "ax", @progbits
	.global	_start_marshalling
	.global	_end_marshalling
	.global _extended_memory_start
	.global _extended_memory_end

	; this code will be relocated to 0x200005
	; as it does not contain JP/CALL within itself
	; we only need to copy the code to the new location

	; ORG 0x200000
_start_marshalling:
	JP	_exit			; 4 bytes

	.space 5-(.-_start_marshalling)	;skip 1 byte

	; ORG $xx0005
_start_cpm_bdos_adl:
	CALL.sis cpm_bdos_z80 & 0xFFFF
	RET

	;ORG $xx000A
_extended_memory_start:
	D24	0	; replaced at load time

	;ORG $xx000D
_extended_memory_end:
	D24	0	; replaced at load time

	;ORG $xx0010
; hbios_adl:
	CALL.sis hbios_z80 & 0xFFFF
	RET

_end_marshalling:
