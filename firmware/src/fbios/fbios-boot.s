	INCLUDE "..\config.inc"
	INCLUDE "..\rst-28-constants.inc"
	INCLUDE "fbios-vars.inc"

	SEGMENT	CODE
	.ASSUME	ADL=1

	XREF	IMAGE_CBIOS
	XREF	IMAGE_CBIOS_SIZE

	XREF	SPL_BASE

	XREF	fbios_vars
	XREF	firmware_rst_28_hook
	XREF	_print_string
	XREF	_get_boot_sector_lsb
	XREF	FB_SELDSK
	XREF	_fbios_mount_disks
	XREF	_fbios_find_boot_disk

	PUBLIC	_os_boot

_os_boot:
	;
	; ALLOCATE 64K SEGMENT
	;
	LD	B, BF_SYSALLOC
	LD	DE, %10000
	LD	HL, %10000				; allocate a single 64K segment
	CALL.IL	firmware_rst_28_hook

	;
	; ASSIGN TO MBASE
	;
	LD	A, E					; assume allocation worked
	LD	MB, A

	CALL.IL	FB_INIT

	XOR	A
	SBC	HL, HL					; clear HL to request
	CALL.IL	FB_RELOAD				; reloading of ccp/bdos/cbios

	;
	; SET OS LOAD GUARD
	;
	LD	IY, (fbios_vars)			; MARK FLAG TO INDICATE OS LOADED
	LD	A, 3
	LD	(IY+FB_OSLOADGUARD), A

	;
	; SET STACK POINTS
	;
	LD	SP, SPL_BASE
	LD	HL, (IY+FB_SYSLOC)
	LD.SIS	SP, HL

	;
	; JUMP TO OS ENTRY POINT (COLD BOOT?)
	;
	LD	IY, (IY+FB_SYSENTRY)
	JP.SIS	(IY)

;---------------------------------------------------------
; Remount all fbios disks & identify boot disk image
;
; If bit 0 of fbios_vars->fb_osloadguard is set, then
; flag is reset and routine does not remount disks
;
; delegates to fbios_mount_disks &
;              fbios_find_boot_disk
;
; Output:     A  = boot disk number
;---------------------------------------------------------

	PUBLIC	FB_INIT
FB_INIT:
	LD	IY, (fbios_vars)
	BIT	0, (IY+FB_OSLOADGUARD)			; TEST FLAG
	JR	Z, FB_INIT1
	RES	0, (IY+FB_OSLOADGUARD)
	RET.L

FB_INIT1:
	CALL	_fbios_mount_disks
	CALL	_fbios_find_boot_disk
	RET.L

;---------------------------------------------------------
; Reload operating system sectors from initial boot
; disk sectors as per boot sector params
;
; see fbios_find_boot_disk
;
; Input:     HL = Number of bytes to reload or 0 to reload
;                 as per boot sector meta
;---------------------------------------------------------

	PUBLIC	FB_RELOAD
FB_RELOAD:
	PUSH	IX
	LD	IX, (fbios_vars)
	BIT	1, (IX+FB_OSLOADGUARD)			; TEST FLAG
	JR	Z, FB_RELOAD1
	RES	1, (IX+FB_OSLOADGUARD)
	POP	IX
	RET.L

FB_RELOAD1:
	PUSH	HL					; Save requested size to reload

	LD	HL, (IX+FB_BOOTSEC)
	LD	C, (IX+FB_BOOTDISK)
	LD	B, BF_DIOSEEK
	CALL.IL	firmware_rst_28_hook

	POP	DE					; test for 0 and divide by 512
	LD	E, D					; DE=DE>>8
	LD	D, 0					; High byte becomes 0
	SRL	E					; DE=DE>>1
	JR	NZ, FB_RELOAD2				; IF 0, then load FB_SYSSECSIZE
	LD	E, (IX+FB_SYSSECSIZE)			; sector_count
FB_RELOAD2:
	LD	HL, (IX+FB_SYSLOC)			; LD HL, <MB>D000
	LD	C, (IX+FB_BOOTDISK)
	LD	B, BF_DIOREAD
	CALL.IL	firmware_rst_28_hook


	; overwrite cbios version loaded from disk
	; for debugging only
	; expect in normal release to not do this.
	;
	; FBIOS OVERRIDE CBIOS
	;
	LD	DE, %E600				; LD DE, <MB>E600
	PUSH	DE
	LD	IY, 0
	ADD	IY, SP
	LD	A, MB
	LD	(IY+2), A
	POP	DE
	LD	HL, IMAGE_CBIOS
	LD	BC, IMAGE_CBIOS_SIZE
	LDIR

	POP	IX
	RET.L

