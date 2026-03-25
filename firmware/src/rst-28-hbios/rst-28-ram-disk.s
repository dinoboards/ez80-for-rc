	include "../config.inc"
	include "../rst-28-constants.inc"
	include "../rst-28-vars.inc"

	SECTION CODE

	.ASSUME ADL=1

	XREF	get_hbios_dio_driver

;---------------------------------------------------------
; RST-28 HBIOS RAM DISK Implementation of DIOSEEK
;
; Marshalls to the C function ram_disk_dio_seek
;
; Input:     C  = Disk unit
;            HL = Sector Number to seek to
; Output:    A  = HBIOS Status Code
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_ram_disk_dioseek
	XREF	_ram_disk_dio_seek
_ram_disk_dioseek:
	PUSH	HL
	CALL	get_hbios_dio_driver
	LEA	IY, IY+DIO_STATE
	PUSH	IY
	CALL	_ram_disk_dio_seek
	POP	BC
	POP	HL
	RET.L

;---------------------------------------------------------
; RST-28 HBIOS RAM DISK Implementation of DIOREAD
;
; Marshalls to the C function ram_disk_dio_read
;
; Input:     C  = Disk unit
;            E  = number of sectors to read
;            HL = buffer address
; Output:    A  = HBIOS Status Code
;            E  = number of sectors actually read
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_ram_disk_dioread
	XREF	_ram_disk_dio_read
_ram_disk_dioread:
	PUSH	HL
	PUSH	DE		; sector_count
	CALL	get_hbios_dio_driver
	LEA	IY, IY+DIO_STATE
	PUSH	IY
	CALL	_ram_disk_dio_read
	POP	BC
	POP	BC
	POP	BC
	LD	A, L
	LD	E, H
	RET.L

;---------------------------------------------------------
; RST-28 HBIOS RAM DISK Implementation of DIOWRITE
;
; Marshalls to the C function ram_disk_dio_write
;
; Input:     C  = Disk unit
;            E  = number of sectors to write
;            HL = buffer address
; Output:    A  = HBIOS Status Code
;            E  = number of sectors actually written
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_ram_disk_diowrite
	XREF	_ram_disk_dio_write
_ram_disk_diowrite:
	PUSH	HL
	PUSH	DE		; sector_count
	CALL	get_hbios_dio_driver
	LEA	IY, IY+DIO_STATE
	PUSH	IY
	CALL	_ram_disk_dio_write
	POP	BC
	POP	BC
	POP	BC
	LD	A, L
	LD	E, H
	RET.L

;---------------------------------------------------------
; RST-28 RAM DISK HBIOS Implementation of DIODEVICE
;
;
; Input:     C  = Disk unit
; Output:    A  = HBIOS Status Code
;            C  = Device Attributes
;	     D  = Device Type
;            E  = Device Number
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_ram_disk_device
_ram_disk_device:
	; Bit 5, 4, + 3
	LD	C, 00010101B
	LD	D, DIODEV_RF
	LD	E, 0

	XOR	A
	RET.L


;---------------------------------------------------------
; RST-28 RAM DISK HBIOS Implementation of DIOMEDIA
;
;
; Input:     C  = Disk unit
;            E  = Flags
; Output:    A  = HBIOS Status Code
;            E  = Media ID
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_ram_disk_media
_ram_disk_media:
	LD	E, MID_RF
	XOR	A
	RET.L

;---------------------------------------------------------
; RST-28 USB SCSI HBIOS Implementation of DIOCAPACITY
;
; Input:     C  = Disk unit
; Output:    A  = HBIOS Status Code
;            HL = Sector Count
;	     BC = Sector Size
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_ram_disk_capacity
_ram_disk_capacity:
	LD	BC, 512
	LD	HL, 1024	; 512KB
	RET.L
