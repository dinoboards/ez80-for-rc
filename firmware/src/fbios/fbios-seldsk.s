	INCLUDE "..\config.inc"
	INCLUDE "fbios-vars.inc"
	INCLUDE "..\rst-28-constants.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

	XREF	fbios_vars
	XREF	_fb_set_storage_unit

;---------------------------------------------------------
; Implement CBIOS SELDSK BIOS Function (function 9)
;
; Select the disc drive in register C (0=A:, 1=B: ...).
;
; Output:     A  = 0 for OK, otherwise ERR_NOUNIT if C is an invalid disk
;                  number.
;             HL = points to the stored reference of the selected storage
;                  unit. (Typically the DPH)
;---------------------------------------------------------
	XREF	_fbios_vars
	PUBLIC	FB_SELDSK
FB_SELDSK:
	LD	A, C
	CP	FBIOS_MAX_STORAGE_UNITS
	JR	NC, invalid_disk			; if A >= FBIOS_MAX_STORAGE_UNITS, then return error

	; fbios_vars->fb_disk = fb_disk;
	LD	IY, (_fbios_vars)
	LD	(IY+FB_TRCK), A

	; fbios_vars->fb_current_storage = &fbios_vars->fb_storage_units[fb_disk];
	UEXT	HL
	LD	L, A
	ADD	HL, HL					; FSTUN_SIZE is 16 bytes
	ADD	HL, HL
	ADD	HL, HL
	ADD	HL, HL
	LD	BC, HL
	LEA	HL, IY+FB_STUN
	ADD	HL, BC
	LD	(IY+FB_CSTUN), HL

	PUSH	HL
	POP	IY					; cb_storage_unit[fb_disk] address into IY

	LD	HL, (IY+FS_DISK_REFERENCE)

	XOR	A
	RET.L

invalid_disk:
	UEXT	HL
	LD	A, ERR_NOUNIT
	OR	A
	RET.L

