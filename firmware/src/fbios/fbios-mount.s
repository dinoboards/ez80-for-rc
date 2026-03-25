	INCLUDE "..\config.inc"
	INCLUDE "..\rst-28-constants.inc"
	INCLUDE "fbios-vars.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

	XREF	fbios_vars
	XREF	_fbios_mount_report

;---------------------------------------------------------
; Implement FBIOS MOUNT Function
;
; This function iterates through previously identified FBIOS disk images.
; It provides the CBIOS the ability to determine if it wants to mount the
; disk within the operating system and supply a reference address.
;
; The iteration is controlled by the A register. On entry set A to 0xFF
; then keep calling FB_MOUNT until A is returned as 0xFF.
;
; On First Call:
; Input:
;                A  = Iteration control. 0xFF to restart iteration.
;
; On subsequent calls:
; Input:
;                A  = Iteration. Must supply the value returned from
;                     previous invocation.
;                C  = fbios disk unit number. Must supply the value
;                     returned from previous invocation.
;               HL  = A 16 bit value reference (eg DPH...) to be stored
;                     against the disk unit.
;               DE  = The number of cp/m sectors per track to assume for
;                     this disk unit.
;
; Output:
;                A  = Iteration. 0xFF end of iteration. 0x00 iteration
;                     continues.
;                C  = The next fbios disk unit number.
;---------------------------------------------------------
	PUBLIC	FB_MOUNT
FB_MOUNT:
	LD	IY, (fbios_vars)
	CP	%FF					; test for iteration start
	JR	NZ, FB_MOUNT2

	; start iteration
	; return B = fbios_vars->fb_storage_units[0].hbios_media_type
	;        A = 0
	;        C = 0
	LD	B, (IY+FB_STUN+FS_HBIOS_MEDIA_TYPE)
	XOR	A
	LD	C, A
	RET.L

FB_MOUNT2:
	PUSH	DE					; save sector per track
	PUSH	HL					; Save supplied reference

	UEXT	HL
	LD	L, C
	ADD	HL, HL					; FSTUN_SIZE is 16 bytes
	ADD	HL, HL
	ADD	HL, HL
	ADD	HL, HL
	EX	HL, DE
	LEA	HL, IY+FB_STUN
	ADD	HL, DE
	LD	IY, HL					; &fbios_vars->cb_storage_unit[disk] address into IY

	POP	HL					; retrieved supplied reference
	POP	DE					; DE = number of sectors per track (spt)

	; Install the disk into FBIOS list

	LD	(IY+FS_DISK_REFERENCE), L
	LD	(IY+FS_DISK_REFERENCE+1), H
	LD	(IY+FS_SECTORS_PER_TRACK), E
	LD	(IY+FS_SECTORS_PER_TRACK+1), D

	INC	C					; iterate to next unit number

	LD	A, C
	CP	FBIOS_MAX_STORAGE_UNITS
	JR	NC, invalid_disk			; if A >= FBIOS_MAX_STORAGE_UNITS, then return error

	; LOAD MEDIA TYPE FOR NEXT UNIT
	LD	DE, FSTUN_SIZE
	ADD	IY, DE
	LD	B, (IY+FS_HBIOS_MEDIA_TYPE)

	XOR	A					; return status ok
	RET.L

invalid_disk:
	CALL	_fbios_mount_report

	LD	A, 0FFh					; end iteration
	LD	IY, (fbios_vars)
	LD	C, (IY+FB_BOOTDISK)
	RET.L
