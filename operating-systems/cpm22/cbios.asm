
	INCLUDE	"../../firmware/src/rst-28-constants.inc"


	section	.code, "ax"
	.assume	adl=0

IOBYTE		EQU	3				; LOC IN PAGE 0 OF I/O DEFINITION BYTE
CDISK		EQU	4				; LOC IN PAGE 0 OF CURRENT DISK NUMBER 0=A,...,15=P

BDOS_LOC	EQU	$D800
CCP_LOC		EQU	$D000
CBIOS_LOC	EQU	$E600
CCP_SIZE	EQU	BDOS_LOC-CCP_LOC

; ADL ENTRY POINTS
CA_VERSION	EQU	$000180

CA_MOUNT	EQU	CA_VERSION+4
CA_RESV1	EQU	CA_VERSION+8
CA_RESV2	EQU	CA_VERSION+12
CA_RESV3	EQU	CA_VERSION+16

CA_INIT		EQU	CA_VERSION+20
CA_RELOAD	EQU	CA_VERSION+24
CA_CONST	EQU	CA_VERSION+28
CA_CONIN	EQU	CA_VERSION+32
CA_CONOUT	EQU	CA_VERSION+36
CA_LIST		EQU	CA_VERSION+40
CA_PUNCH	EQU	CA_VERSION+44
CA_READER	EQU	CA_VERSION+48
CA_HOME		EQU	CA_VERSION+52
CA_SELDSK	EQU	CA_VERSION+56
CA_SETTRK	EQU	CA_VERSION+60
CA_SETSEC	EQU	CA_VERSION+64
CA_SETDMA	EQU	CA_VERSION+68
CA_READ		EQU	CA_VERSION+72
CA_WRITE	EQU	CA_VERSION+76
CA_LISTST	EQU	CA_VERSION+80
CA_SECTRN	EQU	CA_VERSION+84

;==================================================================================================
;	CP/M JUMP VECTOR TABLE FOR INDIVIDUAL SUBROUTINES
;==================================================================================================
; These jumps are defined in the CP/M-80 v2.2 system guide and comprise
; the invariant part of the BIOS.

	.global	_cbios
_cbios:

	JP	_BOOT			; #0  - COLD START
WBOOTE:	JP	_WBOOT			; #1  - WARM START
	JP	_CONST			; #2  - CONSOLE STATUS
	JP	_CONIN			; #3  - CONSOLE CHARACTER IN
	JP	_CONOUT			; #4  - CONSOLE CHARACTER OUT
	JP	_LIST			; #5  - LIST CHARACTER OUT
	JP	_PUNCH			; #6  - PUNCH CHARACTER OUT
	JP	_READER			; #7  - READER CHARACTER IN
	JP	_HOME			; #8  - MOVE HEAD TO HOME POSITION
	JP	_SELDSK			; #9  - SELECT DISK
	JP	_SETTRK			; #10 - SET TRACK NUMBER
	JP	_SETSEC			; #11 - SET SECTOR NUMBER
	JP	_SETDMA			; #12 - SET DMA ADDRESS
	JP	_READ			; #13 - READ DISK
	JP	_WRITE			; #14 - WRITE DISK
	JP	_LISTST			; #15 - retURN LIST STATUS
	JP	_SECTRN			; #16 - SECTOR TRANSLATE

_BOOT:
	CALL.IL	CA_INIT
	XOR	A
	SBC	HL, HL					; clear HL to request
	CALL.IL	CA_RELOAD				; reloading of ccp/bdos/cbios

	CALL	RESCPM

	; mount disks
	LD	A, 0FFh
	CALL.IL	CA_MOUNT				; RETRIEVE DISK UNIT (C) & MEDIA TYPE (B)

diskmnt:
	CP	0FFh
	JR	Z, finished

	LD	A, B
	ADD	A, A
	LD	E, A
	LD	D, 0
	LD	HL, DPBMAP
	ADD	HL, DE
	LD	HL, (HL)

	LD	A, H
	OR	L
	JR	NZ, diskmnt2

	CALL.IL	CA_MOUNT				; No Disk.  iterate to next disk
	JR	diskmnt

diskmnt2:
	LD	DE, (HL)				; retrieve the sectors per track
	CALL	INIT_DISK				; ALLOCATE DPH BUFFERS, RETURNED IN HL
	CALL.IL	CA_MOUNT				; SUPPLY DPH TO MOUNT THE DISK DETAILS
	JR	diskmnt

finished:
	JP	CCP_LOC					; JUMP TO COMMAND PROCESSOR






_WBOOT:
	CALL	RESCPM
	LD	HL, CCP_SIZE				; set HL to size OF CCP to request
	CALL.IL	CA_RELOAD				; reloading of ccp only
	LD	A, (CDISK)
	AND	0Fh
	LD	C, A
	JP	CCP_LOC					; JUMP TO COMMAND PROCESSOR

_CONST:
	LD	A, (IOBYTE)
	CALL.IL	CA_CONST
	ret

_CONIN:
	LD	A, (IOBYTE)
	CALL.IL	CA_CONIN
	ret

_CONOUT:
	LD	A, (IOBYTE)
	CALL.IL	CA_CONOUT
	ret

_LIST:
	CALL.IL	CA_LIST
	ret

_PUNCH:
	CALL.IL	CA_PUNCH
	ret

_READER:
	CALL.IL	CA_READER
	ret

_HOME:
	CALL.IL	CA_HOME
	ret

; Select the disc drive in register C (0=A:, 1=B: ...). Called with E=0 or 0FFFFh.
; If bit 0 of E is 0, then the disc is logged in as if new; if the format has to be
; determined from the boot sector, for example, this will be done.
; If bit 0 if E is 1, then the disc has been logged in before. The disc is not accessed;
; the DPH address (or zero) is returned immediately.
; Under CP/M 2.0 and later, SELDSK returns the address of a Disc Parameter Header in HL.
; If the disc could not be selected it returns HL=0.

_SELDSK:
	CALL.IL	CA_SELDSK
	RET

_SETTRK:
	CALL.IL	CA_SETTRK
	ret

 ; SETSEC (function 11)
 ; Set the sector in BC. Under CP/M 1 and 2 a sector is 128 bytes. Under CP/M 3 the sector size is given in the Disk Parameter Block.
 ; Under CP/M 1.x, the parameter is a byte (passed in C). Under CP/M 2 and later, it is a word.
_SETSEC:
	PUSH.L	BC					; clear uBC upper byte
	LD.LIL 	IY, 0
	ADD.L	IY, SP
	XOR	A
	LD.LIL 	(IY+2), A
	POP.L	BC
	CALL.IL	CA_SETSEC
	RET

_SETDMA:
	PUSH.L	BC
	LD.LIL 	IY, 0
	ADD.L	IY, SP
	.assume adl=1
	LD 	A, MB
	.assume adl=0
	LD.LIL 	(IY+2), A
	POP.L	BC
	CALL.IL	CA_SETDMA
	RET

_READ:
	CALL.IL	CA_READ
	ret

_WRITE:
	CALL.IL	CA_WRITE
	ret

_LISTST:
	CALL.IL	CA_LISTST
	ret

_SECTRN:
	ld	h, b
	ld	l, c
	; ld	hl, bc
	ret

_RESCPM:
RESCPM	EQU	_RESCPM
	LD	A, 0C3H					; LOAD A WITH 'JP' INSTRUCTION (USED BELOW)

	; CPU RESET / RST 0 / JP 0 -> WARM START CP/M
	LD	IX, 0
	LD	(IX+0), A				; JP OPCODE GOES HERE
	LD	HL, WBOOTE				; GET WARM BOOT ENTRY ADDRESS
	LD	(IX+1), HL				; AND PUT IT AT $0001

	; CALL 5 -> INVOKE BDOS
	LD	(IX+5), A				; JP OPCODE AT $0005
	LD	HL, BDOS_LOC + 6			; GET BDOS ENTRY ADDRESS
	LD	(IX+6), HL				; PUT IT AT $0006

	RET

; ALLOCATE BUFFERS FOR A NEW DISK
; INPUT: HL => DPB,
; OUTPUT: HL => DPH

SIZEOF_DPH	EQU	16
INIT_DISK:
	PUSH	HL
	POP	IX

	LD	HL, SIZEOF_DPH
	CALL	ALLOC
	JR	NZ, INIT_DISK_FAIL
	PUSH	HL
	POP	IY

	; IX => DPB
	; IY -> DPH
	OR	A
	SBC	HL, HL
	LD	(IY+0), HL
	LD	(IY+2), HL
	LD	(IY+4), HL
	LD	(IY+6), HL
	LD	(IY+8), dirbuf & 0xFF			; DIRBUF
	LD	(IY+9), dirbuf >> 8			; DIRBUF
	LD	(IY+10), IX				; DPB

	LD	HL, (IX+11)				; HL => CKS

	CALL	ALLOC
	JR	NZ, INIT_DISK_FAIL
	LD	(IY+12), HL				; CSV

	LD	HL, (IX+5)				; DSM

	; HL = (DSM / 8) + 1
	SRL	H
	RR	L					; Shift right 1
	SRL	H
	RR	L					; Shift right 2
	SRL	H
	RR	L					; Shift right 3 (DSM / 8)
	INC	HL					; +1

NO_ALV_CY:
	CALL	ALLOC          				; Returns HL = Address
	JR	NZ, INIT_DISK_FAIL
	LD	(IY+14), HL       			; Store ALV address in DPH
	PUSH	IY
	POP	HL
	RET

INIT_DISK_FAIL:
	XOR	A
	SBC	HL, HL
	RET

; -------------------------------------------------------------------------
; ALLOC: Bump allocator with bounds checking
; Input:  HL = Number of bytes to allocate
; Output: HL = Address of allocated block, or 0 (0000h) if out of memory,
;              or 0000h if requested size was 0000h.
;         Z => allocation success or requested 0 bytes allocated
;         NZ -> allocation failed (OUT OF MEMORY)
; -------------------------------------------------------------------------
ALLOC:
	LD	A, H
	OR	L
	RET	Z			; Return 0 if HL is 0

	PUSH	DE

	LD	DE, (HEAP_PTR)		; DE = Current top of heap
	PUSH	DE			; Save start address for later

	ADD	HL, DE			; HL = New potential top of heap

	; Check if HL > HEAP_END
	LD	DE, HEAP_END
	AND	A			; Clear Carry
	PUSH	HL			; Save new top
	SBC	HL, DE			; HL = NewTop - HEAP_END
	POP	HL			; Restore NewTop into HL

	JR	C, ALLOC_OK		; If NewTop < HEAP_END
	JR	Z, ALLOC_OK		; If NewTop == HEAP_END

	POP	DE			; Clean up saved start address
	XOR	A
	SBC	HL, HL
	OR	0FFh			; NZ FLAG means no allocation
	JR	ALLOC_EXIT

ALLOC_OK:
	LD	(HEAP_PTR), HL		; Update heap pointer
	POP	HL			; HL = Original start address
	XOR	A			; Z FLAG means allocation OK

ALLOC_EXIT:
	POP	DE
	RET


DPBMAP:
	.DW	0		; MID_NONE (NO MEDIA)
	.DW	0		; MID_MDROM
	.DW	0; DPB_RAM		; MID_MDRAM
	.DW	DPB_RF		; MID_RF
	.DW	0;DPB_HD		; MID_HD
	.DW	DPB_FD720	; MID_FD720
	.DW	DPB_FD144	; MID_FD144
	.DW	DPB_FD360	; MID_FD360
	.DW	DPB_FD120	; MID_FD120
	.DW	0 ;DPB_FD111	; MID_FD111
	.DW	DPB_HDNEW	; MID_HDNEW (1024 DIR ENTRIES)
;
DPBCNT	.EQU	($ - DPBMAP) / 2

; DISK PARAMETER BLOCKS
;__________________________________________________________________________________________________
;
; RAM DISK: 64 SECS/TRK, 128 BYTES/SEC
; BLOCKSIZE (BLS) = 2K, DIRECTORY ENTRIES = 256
; RAM DISK SIZE = 512K
;
DPB_RF:
	.DW	64		; SPT: SECTORS PER TRACK
	.DB	4		; BSH: BLOCK SHIFT FACTOR
	.DB	15		; BLM: BLOCK MASK
	.DB	0		; EXM: EXTENT MASK
	.DW	256-1		; DSM: 256*2K=512K
	.DW	255		; DRM: DIR ENTRIES - 1 = 256 - 1 = 255
	.DB	11110000B	; AL0: DIR BLK BIT MAP, FIRST BYTE
	.DB	00000000B	; AL1: DIR BLK BIT MAP, SECOND BYTE
	.DW	0		; CKS: ZERO FOR NON-REMOVABLE MEDIA
	.DW	0		; OFF: RESERVED TRACKS = 0 TRK

;__________________________________________________________________________________________________
;
; GENERIC HARD DISK DRIVE (8MB DATA SPACE + 128K RESERVED SPACE)
;   LOGICAL: 1040 TRKS (16 RESERVED), 64 SECS/TRK, 128 BYTES/SEC
;   PHYSICAL: 65 CYLS (1 RESERVED), 16 HEADS/CYL, 16 SECS/TRK, 512 BYTES/SEC
;   BLOCKSIZE (BLS) = 4K, DIRECTORY ENTRIES = 1024
;
DPB_HDNEW:
	.DW	64		; SPT: SECTORS PER TRACK
	.DB	5		; BSH: BLOCK SHIFT FACTOR
	.DB	31		; BLM: BLOCK MASK
	.DB	1		; EXM: EXTENT MASK
	.DW	2048 - 1 - 4	; DSM: STORAGE BLOCKS - 1 - RES TRKS
	.DW	1024 - 1	; DRM: DIR ENTRIES - 1
	.DB	11111111B	; AL0: DIR BLK BIT MAP, FIRST BYTE
	.DB	00000000B	; AL1: DIR BLK BIT MAP, SECOND BYTE
	.DW	0		; CKS: DIRECTORY CHECK VECTOR SIZE = 256 / 4
	.DW	2		; OFF: RESERVED TRACKS
;__________________________________________________________________________________________________
;
; IBM 720KB 3.5" FLOPPY DRIVE, 80 TRKS, 36 SECS/TRK, 512 BYTES/SEC
; BLOCKSIZE (BLS) = 2K, DIRECTORY ENTRIES = 128
;
DPB_FD720:
	.DW	36		; SPT: SECTORS PER TRACK
	.DB	4		; BSH: BLOCK SHIFT FACTOR
	.DB	15		; BLM: BLOCK MASK
	.DB	0		; EXM: EXTENT MASK
	.DW	350		; DSM: TOTAL STORAGE IN BLOCKS - 1 BLK = ((720K - 18K OFF) / 2K BLS) - 1 = 350
	.DW	127		; DRM: DIR ENTRIES - 1 = 128 - 1 = 127
	.DB	11000000B	; AL0: DIR BLK BIT MAP, FIRST BYTE
	.DB	00000000B	; AL1: DIR BLK BIT MAP, SECOND BYTE
	.DW	32		; CKS: DIRECTORY CHECK VECTOR SIZE = 128 / 4
	.DW	4		; OFF: RESERVED TRACKS = 4 TRKS * (512 B/SEC * 36 SEC/TRK) = 18K
;__________________________________________________________________________________________________
;
; IBM 1.44MB 3.5" FLOPPY DRIVE, 80 TRKS, 72 SECS/TRK, 512 BYTES/SEC
; BLOCKSIZE (BLS) = 2K, DIRECTORY ENTRIES = 256
;
DPB_FD144:
	.DW	72		; SPT: SECTORS PER TRACK
	.DB	4		; BSH: BLOCK SHIFT FACTOR
	.DB	15		; BLM: BLOCK MASK
	.DB	0		; EXM: EXTENT MASK
	.DW	710		; DSM: TOTAL STORAGE IN BLOCKS - 1 BLK = ((1,440K - 18K OFF) / 2K BLS) - 1 = 710
	.DW	255		; DRM: DIR ENTRIES - 1 = 256 - 1 = 255
	.DB	11110000B	; AL0: DIR BLK BIT MAP, FIRST BYTE
	.DB	00000000B	; AL1: DIR BLK BIT MAP, SECOND BYTE
	.DW	64		; CKS: DIRECTORY CHECK VECTOR SIZE = 256 / 4
	.DW	2		; OFF: RESERVED TRACKS = 2 TRKS * (512 B/SEC * 72 SEC/TRK) = 18K
;__________________________________________________________________________________________________
;
; IBM 360KB 5.25" FLOPPY DRIVE, 40 TRKS, 9 SECS/TRK, 512 BYTES/SEC
; BLOCKSIZE (BLS) = 2K, DIRECTORY ENTRIES = 128
;
DPB_FD360:
	.DW	36		; SPT: SECTORS PER TRACK
	.DB	4		; BSH: BLOCK SHIFT FACTOR
	.DB	15		; BLM: BLOCK MASK
	.DB	1		; EXM: EXTENT MASK
	.DW	170		; DSM: TOTAL STORAGE IN BLOCKS - 1 BLK = ((360K - 18K OFF) / 2K BLS) - 1 = 170
	.DW	127		; DRM: DIR ENTRIES - 1 = 128 - 1 = 127
	.DB	11110000B	; AL0: DIR BLK BIT MAP, FIRST BYTE
	.DB	00000000B	; AL1: DIR BLK BIT MAP, SECOND BYTE
	.DW	32		; CKS: DIRECTORY CHECK VECTOR SIZE = 128 / 4
	.DW	4		; OFF: RESERVED TRACKS = 4 TRKS * (512 B/SEC * 36 SEC/TRK) = 18K
;__________________________________________________________________________________________________
;
; IBM 1.20MB 5.25" FLOPPY DRIVE, 80 TRKS, 15 SECS/TRK, 512 BYTES/SEC
; BLOCKSIZE (BLS) = 2K, DIRECTORY ENTRIES = 256
;
DPB_FD120:
	.DW	60		; SPT: SECTORS PER TRACK
	.DB	4		; BSH: BLOCK SHIFT FACTOR
	.DB	15		; BLM: BLOCK MASK
	.DB	0		; EXM: EXTENT MASK
	.DW	591		; DSM: TOTAL STORAGE IN BLOCKS - 1 BLK = ((1,200K - 15K OFF) / 2K BLS) - 1 = 591
	.DW	255		; DRM: DIR ENTRIES - 1 = 256 - 1 = 255
	.DB	11110000B	; AL0: DIR BLK BIT MAP, FIRST BYTE
	.DB	00000000B	; AL1: DIR BLK BIT MAP, SECOND BYTE
	.DW	64		; CKS: DIRECTORY CHECK VECTOR SIZE = 256 / 4
	.DW	2		; OFF: RESERVED TRACKS = 2 TRKS * (512 B/SEC * 60 SEC/TRK) = 15K
;__________________________________________________________________________________________________
;
; IBM 1.11MB 8" FLOPPY DRIVE, 77 TRKS, 15 SECS/TRK, 512 BYTES/SEC
; BLOCKSIZE (BLS) = 2K, DIRECTORY ENTRIES = 256
;
PB_FD111:
	.DW	60		; SPT: SECTORS PER TRACK
	.DB	4		; BSH: BLOCK SHIFT FACTOR
	.DB	15		; BLM: BLOCK MASK
	.DB	0		; EXM: EXTENT MASK
	.DW	569		; DSM: TOTAL STORAGE IN BLOCKS - 1 BLK = ((1,155K - 15K OFF) / 2K BLS) - 1 = 569
	.DW	255		; DRM: DIR ENTRIES - 1 = 256 - 1 = 255
	.DB	11110000B	; AL0: DIR BLK BIT MAP, FIRST BYTE
	.DB	00000000B	; AL1: DIR BLK BIT MAP, SECOND BYTE
	.DW	64		; CKS: DIRECTORY CHECK VECTOR SIZE = 256 / 4
	.DW	2		; OFF: RESERVED TRACKS = 2 TRKS * (512 B/SEC * 60 SEC/TRK) = 15K

	section	.data

	XREF	HEAP_END
	XREF	HEAP
HEAP_PTR:
	DW	HEAP					; Initialize this to your heap start address

	section	.bss

dirbuf:
	DS	128






