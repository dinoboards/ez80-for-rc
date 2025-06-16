
.macro	EZ80_FN
	RST.L	$10
.endm

.macro	EZ80_EMULATOR_INVOKE addr
	ld	a, 7
	ld	iy, \addr
	EZ80_FN
.endm

.macro SYSUTL_EMULSTAT_GET
	xor	a
	ld	b, 19
	EZ80_FN
.endm

	.assume	adl=1
	section	.text, "ax", @progbits
	.extern cpm_bdos_z80
	.extern	_emulator_detected
	global	cpm_bdos_adl

_support_emulation:
	ld	a, 1
	ld	(_emulator_detected), a
	; ensure calls to bdos will be executed with the emulator
	ld	de, cpm_bdos_adl
	ld	hl, cpm_bdos_adl_emulator
	ld	bc, cpm_bdos_adl_emulator_size
	ldir
	ret.l

cpm_bdos_adl:
cpm_bdos_adl_native:
	; if running native z80 execution
	CALL.sis cpm_bdos_z80 & 0xFFFF
	RET

	; cpm_bdos_adl_emulator will be copied over cpm_bdos_adl_native
	; if emulator execution is detected.
cpm_bdos_adl_emulator:
	; if running emulator z80 execution
	EZ80_EMULATOR_INVOKE cpm_bdos_z80
	RET
cpm_bdos_adl_emulator_end:

cpm_bdos_adl_emulator_size	equ	cpm_bdos_adl_emulator_end-cpm_bdos_adl_emulator

;;;;;;;;;;;;;;;;;;;

	.assume	adl=0
	section	.startup_z80_init, "ax", @progbits
	SYSUTL_EMULSTAT_GET
	cp	2
	call.il	z, _support_emulation

	end

