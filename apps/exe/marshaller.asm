
	.assume	adl=1
	section	.text, "ax", @progbits
	.global	_start_marshalling
	.global	_end_marshalling

	; this code will be relocated to 0x200005
	; as it does not contain JP/CALL within itself
	; we only need to copy the code to the new location

	; ORG 0x200000
_start_marshalling:
	JP	_exit

	.space 5-(.-_start_marshalling)

	; ORG 0x200005
_start_cpm_bdos_adl:
	CALL.sis cpm_bdos_z80 & 0xFFFF
	RET
_end_cpm_bdos_adl:
_end_marshalling:

	.global	_main_func
	_main_func	= 0x200400