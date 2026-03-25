	include "../config.inc"
	include "../rst-28-constants.inc"
	include "../rst-28-vars.inc"

	SECTION CODE

	.ASSUME ADL=1

	XREF	_hbios_vars

	PUBLIC	get_hbios_dio_driver

;---------------------------------------------------------
; Get the specific dio_driver_t record for the
; disk unit (C) and return it in IY
;
; Input:     C  = Disk unit index
; Output:    IY = Pointer to disk driver structure,
;                 or 0 if C >= MAX_HBIOS_DIO_INSTANCES
;            A  = ERR_OK or ERR_NOUNIT
;            Z  = driver found
;	     NZ = no driver for requested unit
; Clobbered: BC', HL'
;---------------------------------------------------------
get_hbios_dio_driver:
	LD	A, C
	CP	MAX_HBIOS_DIO_INSTANCES	; Check bounds
	JR	C, .within_bounds		; If C < MAX, proceed
	LD	IY, 0				; Otherwise, return 0
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
	LEA	IY, IY+HB_DIO_DRIVERS			; position of dio_drivers in hbios_vars
	EX	HL, DE
	ADD	IY, DE
	EXX
	LD	A, (IY+DIO_INSTANCE)
	OR	A
	JR	Z, not_active
	XOR	A
	RET
not_active:
	XOR	A
	CPL
	OR	A
	RET
