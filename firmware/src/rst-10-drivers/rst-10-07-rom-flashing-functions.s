;
; ROM Flashing Functions
;
; Access with RST.L %10 (A=7, B=sub function code)
;
; The ROM Flashing functions provide the ability to reflash
; the on-chip ROM with a new alt firmware image.
; Guarded to only allow flashing from %010000 to %01FFFF
;
; There are 2 versions built of this driver
; For the main firmware - all functions are availble to flash from %010000 to %01FFFF and the info block
; for the alt firmware - only the info block function are accessable.

        INCLUDE "..\config.inc"

	SECTION CODE

	XREF	_IFL_Init
	XREF	_IFL_IsAddrValid
	XREF	_IFL_GetPage
	XREF	_IFL_ErasePages
	XREF	_IFL_EraseInfoPage
	XREF	_IFL_Program
	XREF	_IFL_ProgramInfoPage
	XREF	_IFL_ReadInfoPage
	XREF	_IFL_WriteInfoByte

	.assume adl=1

	PUBLIC	_rom_flash_dispatch
;
; ROM FLASHING DISPATCH
; Dispatcher for the RST.L %10 trap functions
;
; Inputs:
;  B      = Utils sub function index
;
; Outputs:
;  A	 = 0 -> Success, otherwise errored
;  Other registers as per sub-functions
;
_rom_flash_dispatch:
	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, IFL_Init				; B = 0, IFL_INIT
	DEC	A
	JR	Z, IFL_IsAddrValid			; B = 1, IFL_ISADDRVALID
	DEC	A
	JR	Z, IFL_GetPage				; B = 2, IFL_GETPAGE
	DEC	A
	JR	Z, IFL_ErasePages			; B = 3, IFL_ERASEPAGES
	DEC	A
	JR	Z, IFL_EraseInfoPage			; B = 4, IFL_ERASEINFOPAGE
	DEC	A
	JR	Z, IFL_Program				; B = 5, IFL_PROGRAM
	DEC	A
	JR	Z, IFL_ProgramInfoPage			; B = 6, IFL_PROGRAMINFOPAGE
	DEC	A
	JR	Z, IFL_ReadInfoPage			; B = 7, IFL_READINFOPAGE
	DEC	A
	JR	Z, IFL_WriteInfoByte			; B = 8, IFL_WRITEINFOBYTE

	LD	A, %FF					; UNKNOWN FUNCTION
	RET.L

IFL_Init:
	CALL	_IFL_Init
	RET.L
;
; Function B = 01 -- IFL_ISADDRVALID
; Determines if specified address range resides within internal Flash
;
; Inputs:
;  uHL    = Start address to check
;  DE     = Length of range to check (16bit only)
;
; Outputs:
;  A	 = ZFL_ERR_SUCCESS(0), otherwise Flash Library status code
;
IFL_IsAddrValid:
	PUSH	DE
	PUSH	HL
	CALL	_IFL_IsAddrValid
	POP	HL
	POP	DE
	RET.L
;
; Function B = 02 -- IFL_GETPAGE
; Obtains the page number corresponding to an address within internal Flash
;
; Inputs:
;  HL     = Address within internal Flash
;
; Outputs:
;  A	 = Page number
;
IFL_GetPage:
	PUSH	HL
	CALL	_IFL_GetPage
	POP	HL
	RET.L
;
; Function B = 03 -- IFL_ERASEPAGES
; Erases the specified number of pages within internal Flash
;
; Inputs:
;  HL     = Start address of first page to erase
;  E      = Number of pages to erase
;
; Outputs:
;  A	 = ZFL_ERR_SUCCESS(0), otherwise Flash Library status code
;
IFL_ErasePages:
	PUSH	DE
	PUSH	HL
	CALL	_IFL_ErasePages
	POP	HL
	POP	DE
	RET.L
;
; Function B = 04 -- IFL_ERASEINFOPAGE
; Erases the one of the Information Page
;
; Inputs:
;  L     = Destination address within the Information Page to be erased
;  None
;
; Outputs:
;  A	 = ZFL_ERR_SUCCESS(0), otherwise Flash Library status code
;
IFL_EraseInfoPage:
	PUSH	HL
	CALL	_IFL_EraseInfoPage
	POP	HL
	RET.L
;
; Function B = 05 -- IFL_PROGRAM
; Programs one or more bytes in internal Flash
;
; Inputs:
;  'HL    = Destination address within internal Flash
;  'DE    = Source address in RAM
;  'BC    = Number of bytes to program (16bit)
;
; Outputs:
;  A	 = ZFL_ERR_SUCCESS(0), otherwise Flash Library status code
;
IFL_Program:
	EXX
	PUSH	BC
	PUSH	DE
	PUSH	HL
	CALL	_IFL_Program
	POP	HL
	POP	DE
	POP	BC
	EXX
  	RET.L
;
; Function B = 06 -- IFL_PROGRAMINFOPAGE
; Programs one or more bytes in the Information Page
;
; Inputs:
;  L     = Destination address within the Information Page
;  DE    = Source address in RAM
;  C     = Number of bytes to program
;
; Outputs:
;  A	 = ZFL_ERR_SUCCESS(0), otherwise Flash Library status code
;
IFL_ProgramInfoPage:
	PUSH	BC
	PUSH	DE
	PUSH	HL
	CALL	_IFL_ProgramInfoPage
	POP	HL
	POP	DE
	POP	BC
	RET.L
;
; Function B = 07 -- IFL_READINFOPAGE
; Reads one or more bytes of data from the information page
;
; Inputs:
;  HL    = Destination address in RAM
;  E     = Source address within the Information Page (Is this nor limited to 256 bytes?)
;  C     = Number of bytes to read (Is this nor limited to 256 bytes?)
;
; Outputs:
;  A	 = ZFL_ERR_SUCCESS(0), otherwise Flash Library status code
;
IFL_ReadInfoPage:
	PUSH	BC
	PUSH	DE
	PUSH	HL
	CALL	_IFL_ReadInfoPage
	POP	HL
	POP	DE
	POP	BC
	RET.L
;
; Function B = 08 -- IFL_WRITEINFOBYTE
; Writes a single byte of data to the information page
;
; Inputs:
;  L     = Destination address within the Information Page
;  H     = Data byte to write
;
; Outputs:
;  A	 = ZFL_ERR_SUCCESS(0), otherwise Flash Library status code
;
IFL_WriteInfoByte:
	PUSH	HL
	CALL	_IFL_WriteInfoByte
	POP	HL
	RET.L
