; Implement a HBIOS Compatible Driver Interface

	include "config.inc"
	include "rst-28-constants.inc"

	include "rst-28-vars.inc"

	SECTION CODE

	XREF	firmware_rst_28_hook
	XREF	get_hbios_cio_driver
	XREF	get_hbios_dio_driver

	.assume adl=1

; B = function index
	PUBLIC	_rst_28_functions
_rst_28_functions:
	LD	A, B
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, cioin				; B = 0, CIOIN
	DEC	A
	JR	Z, cioout				; B = 1, CIOOUT
	DEC	A
	JR	Z, cioist				; B = 2, CIOIST

	LD	A, B
	CP	BF_DIOSEEK
	JR	Z, dioseek
	CP	BF_DIOREAD
	JR	Z, dioread
	CP	BF_DIOWRITE
	JR	Z, diowrite
	CP	BF_DIODEVICE
	JR	Z, diodevice
	CP	BF_DIOMEDIA
	JR	Z, diomedia
	CP	BF_DIOCAP
	JR	Z, diocapacity

	CP	BF_SYSALLOC
	JR	Z, sysalloc

rst_28_unknown:
	LD	A, %FF					; UNKNOWN FUNCTION
	RET.L

; --------------------------------------
; CIO Functions
; --------------------------------------

; ### Function 0x00 -- Character Input (CIOIN)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x00                                | A: Status                              |
; | C: Character Unit                      | E: Character                           |
;
; Read and return a Character (E) from the specified Character Unit
; (C).  If no character(s) are available in the unit's input buffer, this
; function will wait indefinitely.  The returned Status (A) is a standard
; HBIOS result code.
cioin:
	CALL	get_hbios_cio_driver
	RET.L	NZ
	LD	IY, (IY+CIO_FUNCS)
	LD	IY, (IY+CIO_IN)
	JP	(IY)


; ### Function 0x01 -- Character Output (CIOOUT)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x01                                | A: Status (0-OK, else error)           |
; | C: Character Unit                      |                                        |
; | E: Character                           |                                        |
;
; Send a Character (E) via the specified Character Unit (C).  If
; there is no space available in the unit's output buffer, the function
; will wait indefinitely.  The returned Status (A) is a standard HBIOS result code.
cioout:
	CALL	get_hbios_cio_driver
	RET.L	NZ
	LD	IY, (IY+CIO_FUNCS)
	LD	IY, (IY+CIO_OUT)
	JP	(IY)


; ### Function 0x02 -- Character Input Status (CIOIST)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x02                                | A: Status / Characters Pending         |
; | C: Character Unit                      |                                        |
;
; Return the count of Characters Pending (A) in the input buffer of the
; specified Character Unit (C).  If the unit has no input buffer or the
; buffer utilization is not available, the function may return simply 0 or
; 1 where 0 means there is no character available and 1 means there is at
; least one character available.
;
; The value returned in register A is used as both a Status (A) code and
; the return value. Negative values (bit 7 set) indicate a standard HBIOS
; result (error) code.  Otherwise, the return value represents the number
; of characters in the input buffer.
cioist:
	CALL	get_hbios_cio_driver
	RET.L	NZ
	LD	IY, (IY+CIO_FUNCS)
	LD	IY, (IY+CIO_IST)
	JP	(IY)

; ### Function 0x12 -- Disk Seek (DIOSEEK)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x12                                | A: Status                              |
; | C: Disk Unit                           |                                        |
; | HL: Sector Address                     |                                        |
;
; This function will set the desired sector to be used for the next I/O
; operation on the specified Disk Unit (C).  The returned Status (A) is a
; standard HBIOS result code.
;
; An actual seek operation is generally not performed on the disk hardware
; by this function.  The function typically just records the sector
; address for subsequent I/O function calls.
;
; The 24bit Sector Address (HL) represent a Logical Block Address (LBA).
;
; ???? Not Implemented yet - For compatibility reason, the high bit is set and the rest of the
; double-word is then treated as the logical sector address.
;
	xref	_dio_seek
	XREF	_get_disk_driver
dioseek:
	CALL	get_hbios_dio_driver
	RET.L	NZ
	LD	IY, (IY+DIO_FUNCS)
	LD	IY, (IY+DIO_SEEK)
	JP	(IY)


; ### Function 0x13 -- Disk Read (DIOREAD)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x13                                | A: Status                              |
; | C: Disk Unit                           | E: Sectors Read                        |
; | E: Sector Count                        |                                        |
; | HL: Buffer Address                     |                                        |
;
; Read Sector Count (E) sectors into the buffer located Address (HL) starting at the
; Current Sector.  The returned Status (A) is a standard HBIOS result code.
;
; The Current Sector is established by a prior DIOSEEK function call;
; however, multiple read/write/verify function calls can be made after a
; seek function. The Current Sector is incremented after each sector
; successfully read. On error, the Current Sector will be the sector where
; the error occurred. Sectors Read (E) indicates the number of sectors
; successfully read.
;
; A Sector Count of zero will result in no sectors being read and a
; status of success.  The buffer will not be modified.
;
; The caller must ensure that the Buffer Address is large enough to
; contain all sectors requested.
;
	xref	_dio_read
dioread:
	CALL	get_hbios_dio_driver
	RET.L	NZ
	LD	IY, (IY+DIO_FUNCS)
	LD	IY, (IY+DIO_READ)
	JP	(IY)


; ### Function 0x14 -- Disk Write (DIOWRITE)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x14                                | A: Status                              |
; | C: Disk Unit                           | E: Sectors Written                     |
; | E: Sector Count                        |                                        |
; | HL: Buffer Address                     |                                        |
;
; Write Sector Count (E) sectors from the buffer located at Buffer Address
; (HL) starting at the Current Sector.  The returned Status (A) is a
; standard HBIOS result code.
;
; The Current Sector is established by a prior DIOSEEK function call;
; however, multiple read/write/verify function calls can be made after a
; seek function. The Current Sector is incremented after each sector
; successfully written. On error, the Current Sector will be the sector
; where the error occurred. Sectors Written (E) indicates the number of
; sectors successfully written.
;
; A Sector Count of zero will result in no sectors being written and a
; status of success.  The buffer will not be modified.
;
	xref	_dio_write
diowrite:
	CALL	get_hbios_dio_driver
	RET.L	NZ
	LD	IY, (IY+DIO_FUNCS)
	LD	IY, (IY+DIO_WRITE)
	JP	(IY)


; ### Function 0x17 -- Disk Device (DIODEVICE)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x17                                | A: Status                              |
; | C: Disk Unit                           | C: Device Attributes                   |
; |                                        | D: Device Type                         |
; |                                        | E: Device Number                       |
;
; Reports device information about the specified Disk Unit (C).  The
; Status (A) is a standard HBIOS result code.

; The Device Attribute (C) value returned indicates various
; feature indicators related to the device being referenced
; by the specified Disk Unit (C).  The high 3 bits apply to
; all devices.  The definition of the low 5 bits depends on
; whether the device is a Floppy (indicated by bit 5).

; The common bits are:

; | **Bits** | **Definition**                                   |
; |---------:|--------------------------------------------------|
; | 7        | Floppy                                           |
; | 6        | Removable                                        |
; | 5        | High Capacity (>8 MB)                            |

; The Floppy specific bits are:

; | **Bits** | **Definition**                                   |
; |---------:|--------------------------------------------------|
; | 4-3      | Form Factor: 0=8", 1=5.25", 2=3.5", 3=Other      |
; | 2        | Sides: 0=SS, 1=DS                                |
; | 1-0      | Density: 0=SD, 1=DD, 2=HD, 3=ED                  |

; The non-Floppy specific bits are:

; | **Bits** | **Definition**                                   |
; |---------:|--------------------------------------------------|
; | 4        | LBA Capable                                      |
; | 3-0      | Media Type: 0=Hard Disk, 1=CF, 2=SD, 3=USB,      |
; |          |   4=ROM, 5=RAM, 6=FLASH, 7=RAMF, 8=CD-ROM,       |
; |          |   9=Cartridge                                    |

; Device Type (D) indicates the specific hardware driver that handles the
; specified Disk Unit (C).  Values are listed at the start of this
; section. Device Number (E) indicates the physical device number assigned
; per driver.  For example, a Device Type of 0x30 with a Device Number
; of 1 refers to the second disk being handled by the IDE driver.

diodevice:
	CALL	get_hbios_dio_driver
	RET.L	NZ
	LD	IY, (IY+DIO_FUNCS)
	LD	IY, (IY+DIO_DEVICE)
	JP	(IY)


; ### Function 0x18 -- Disk Media (DIOMEDIA)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x18                                | A: Status                              |
; | C: Disk Unit                           | E: Media ID                            |
; | E: Flags                               |                                        |
;
; Report the Media ID (E) for the for media in the specified Disk Unit
; (C). If bit 0 of Flags (E) is set, then media discovery or verification
; will be performed.  The Status (A) is a standard HBIOS result code. If
; there is no media in device, function will return an error status.
;
diomedia:
	CALL	get_hbios_dio_driver
	RET.L	NZ
	LD	IY, (IY+DIO_FUNCS)
	LD	IY, (IY+DIO_MEDIA)
	JP	(IY)


; ### Function 0x1A -- Disk Capacity (DIOCAPACITY)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0x1A                                | A: Status                              |
; | C: Disk Unit                           | HL: Sector Count                       |
; |                                        | BC: Sector Size                        |
;
; Report the current media capacity information for the specified Disk Unit (C).
; The Sector Count (HL) is a 24 bit number representing the
; total number of blocks on the device. Sector Size (BC) contains the sector
; size in bytes. The Status (A) is a standard HBIOS result code. If the
; media is unknown, an error will be returned.
;
; This function will not attempt to discover or verify the media loaded in
; the unit specified.  You can use precede this function with the
; DIOMEDIA function to force this if desired.
;
diocapacity:
	CALL	get_hbios_dio_driver
	RET.L	NZ
	LD	IY, (IY+DIO_FUNCS)
	LD	IY, (IY+DIO_CAPACITY)
	JP	(IY)








; ### Function 0xF6 -- System Alloc (SYSALLOC)
;
; | **Entry Parameters**                   | **Returned Values**                    |
; |----------------------------------------|----------------------------------------|
; | B: 0xF6                                | A: Status                              |
; | HL: Block Size                         | HL: Block Address                      |
; | DE: Page Alignment			   | E: Upper byte of UHL
;
; This function will attempt to allocate a Block Size (HL) bytes block of
; memory from the linear RAM. If the allocation is successful, the Block
; Address (HL) of the allocated memory block is returned in HL.
; The Status (A) is a standard HBIOS result code.
	XREF	_sys_alloc
sysalloc:
	PUSH	DE
	PUSH	HL
	CALL	_sys_alloc
	POP	BC
	POP	BC

	PUSH	HL					; save result
	LD	IY, 0
	ADD	IY, SP
	LD	E, (IY+2)				; load e with upper byte
	POP	HL					; restore hl

	XOR	A
	OR	E
	JR	NZ, sysalloc_ok

	XOR	A
	OR	H
	JR	NZ, sysalloc_ok

	XOR	A
	OR	L
	JR	NZ, sysalloc_ok

	LD	A, ERR_NOMEM
	OR	A
	RET.L

sysalloc_ok
	XOR	A
	RET.L
