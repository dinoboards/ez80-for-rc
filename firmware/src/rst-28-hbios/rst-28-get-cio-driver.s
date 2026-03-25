	include "../config.inc"
	include "../rst-28-constants.inc"
	include "../rst-28-vars.inc"

	SECTION CODE

	.ASSUME ADL=1

	XREF	_hbios_vars

	PUBLIC	get_hbios_cio_driver

;---------------------------------------------------------
; Get the specific cio_driver_t record for the
; cio unit (C) and return it in IY
;
; Input:     C  = Disk unit index. 0x80 for default cio
; Output:    IY = Pointer to disk driver structure,
;                 or 0 if C >= MAX_HBIOS_CIO_INSTANCES
;            A  = ERR_OK or ERR_NOUNIT
;            Z  = driver found
;	     NZ = no driver for requested unit
; Clobbered: BC', HL'
;---------------------------------------------------------
get_hbios_cio_driver:
	LD	A, C
	AND	~%80					; Default is 0.  TODO
	CP	MAX_HBIOS_CIO_INSTANCES			; Check bounds
	JR	C, .within_bounds			; If C < MAX, proceed
	LD	IY, 0					; Otherwise, return 0
	LD	A, ERR_NOUNIT
	OR	A
	RET

.within_bounds:
	EXX
	UEXT	HL
	LD	L, A
	ADD	HL, HL					; * 2
	ADD	HL, HL					; * 4
	ADD	HL, HL					; * 8
	ADD	HL, HL					; * 16
	LD	IY, (_hbios_vars)
	LD	DE, HB_CIO_DRIVERS			; position of dio_drivers in hbios_vars
	ADD	IY, DE
	EX	HL, DE
	ADD	IY, DE
	EXX
	LD	A, (IY+CIO_INSTANCE)
	OR	A
	JR	Z, not_active
	XOR	A
	RET
not_active:
	XOR	A
	CPL
	OR	A
	RET
