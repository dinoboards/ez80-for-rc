	include "../config.inc"
	include "../rst-28-constants.inc"
	include "../rst-28-vars.inc"

	SECTION CODE

	.ASSUME ADL=1

	XREF	get_hbios_dio_driver

;---------------------------------------------------------
; RST-28 HBIOS USB SCSI Implementation of DIOSEEK
;
; Marshalls to the C function usb_scsi_dio_seek
;
; Input:     C  = Disk unit
;            HL = Sector Number to seek to
; Output:    A  = HBIOS Status Code
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_usb_scsi_dioseek
	XREF	_usb_scsi_dio_seek
_usb_scsi_dioseek:
	PUSH	HL
	CALL	get_hbios_dio_driver
	LEA	IY, IY+DIO_STATE
	PUSH	IY
	CALL	_usb_scsi_dio_seek
	POP	BC
	POP	HL
	RET.L

;---------------------------------------------------------
; RST-28 HBIOS USB SCSI Implementation of DIOREAD
;
; Marshalls to the C function usb_scsi_dio_read
;
; Input:     C  = Disk unit
;            E  = number of sectors to read
;            HL = buffer address
; Output:    A  = HBIOS Status Code
;            E  = number of sectors actually read
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_usb_scsi_dioread
	XREF	_usb_scsi_dio_read
_usb_scsi_dioread:
	PUSH	HL
	PUSH	DE		; sector_count
	CALL	get_hbios_dio_driver
	LEA	IY, IY+DIO_STATE
	PUSH	IY
	CALL	_usb_scsi_dio_read
	POP	BC
	POP	BC
	POP	BC
	LD	A, L
	LD	E, H
	RET.L

;---------------------------------------------------------
; RST-28 HBIOS USB SCSI Implementation of DIOWRITE
;
; Marshalls to the C function usb_scsi_dio_write
;
; Input:     C  = Disk unit
;            E  = number of sectors to write
;            HL = buffer address
; Output:    A  = HBIOS Status Code
;            E  = number of sectors actually written
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_usb_scsi_diowrite
	XREF	_usb_scsi_dio_write
_usb_scsi_diowrite:
	PUSH	HL
	PUSH	DE		; sector_count
	CALL	get_hbios_dio_driver
	LEA	IY, IY+DIO_STATE
	PUSH	IY
	CALL	_usb_scsi_dio_write
	POP	BC
	POP	BC
	POP	BC
	LD	A, L
	LD	E, H
	RET.L

;---------------------------------------------------------
; RST-28 USB SCSI HBIOS Implementation of DIODEVICE
;
;
; Input:     C  = Disk unit
; Output:    A  = HBIOS Status Code
;            C  = Device Attributes
;	     D  = Device Type
;            E  = Device Number
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_usb_scsi_device
_usb_scsi_device:
	CALL	get_hbios_dio_driver
	LD	C, 00111010B
	LD	D, DIODEV_USB
	LD	E, (IY+DIO_INSTANCE)
	DEC	E
	XOR	A
	RET.L

;---------------------------------------------------------
; RST-28 USB SCSI HBIOS Implementation of DIOMEDIA
;
;
; Input:     C  = Disk unit
;            E  = Flags
; Output:    A  = HBIOS Status Code
;            E  = Media ID
; Protected: IX
;---------------------------------------------------------
	PUBLIC	_usb_scsi_media
_usb_scsi_media:
	LD	E, MID_HDNEW
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
	PUBLIC	_usb_scsi_capacity
	XREF	_usb_scsi_dio_capacity
_usb_scsi_capacity:
	CALL	get_hbios_dio_driver
	LEA	IY, IY+DIO_STATE
	PUSH	IY
	CALL	_usb_scsi_dio_capacity
	POP	BC
	LD	BC, 512
	RET.L
