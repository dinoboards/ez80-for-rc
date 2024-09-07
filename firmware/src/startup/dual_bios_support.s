;
;
; RESERVE RAM 0x02FFFF for BIOS VERSION FLAG (BIOS_FLAG)
; On RST 00:
;  1. Check if alt bios is installed - look for a marker somewhere in 0x01000 to 0x01FFF
;  2. If alt bios is installed:
;    3. if BIOS_FLAG == 0x00 (installed never run), then:
;    4.   Change BIOS_FLAG to 0x01
;    5.   Change RST jumps to alt versions
;
;    6.   Jump to alt bios
;    7. else if BIOS_FLAG == 0x01 (installed and run), then alt boot failed, continue with main bios
;
; On Version Check call:
;  if BIOS_FLAG == 0x01 (then running on alt bios), change BIOS_FLAG to 0x02 (installed and working)
;  if BIOS_FLAG == 0x02 (then running on alt bios), and keep it as is
;  if BIOS_FLAG == 0x00 (then running on main bios), and keep it as is

; RST 08:
;  if BIOS_FLAG == 0x01 (installed and run) or == 0x02 (installed and working), then jump to 0x01008
;  else continue with current implementation

; RST 10:
;  if BIOS_FLAG == 0x01 (installed and run) or == 0x02 (installed and working), then jump to 0x01010
;  else continue with current implementation

        DEFINE	ALT_BIOS_CTRL_ROM, SPACE = ROM
        SEGMENT	ALT_BIOS_CTRL_ROM

        DEFINE	ALT_BIOS_CTRL_RAM, SPACE = RAM, ORG=%02FF00
        SEGMENT	ALT_BIOS_CTRL_RAM

	.assume adl=1

	SEGMENT	CODE

IFDEF RC2014_ALT_FIRMWARE
	ORG	%01FF00

MARKER:
	DB	"RC2014-ALT-BIOS", %00
ELSE

IFNDEF RC2014_ALT_FIRMWARE
	; check if alt bios is present, no stack configured yet
	LD	HL, %01FF00
	LD	DE, STRING_TO_CHECK ; Load the address of the string to check against into DE
	LD	B, 6               ; Length of the string "RC2014" is 6

compare_loop:
	LD	A, (DE)					; Load the expected byte
	CP	(HL)					; Compare it with byte withing alt flash space
	JR	NZ, alt_bios_not_found			; If not equal, jump to alt_bios_not_found
	INC	HL
	INC	DE
	DJNZ	compare_loop

	JP	alt_bios_is_found

STRING_TO_CHECK:
	DB	"RC2014"

ENDIF

alt_bios_is_found:
	NOP
alt_bios_not_found:

ENDIF

	SEGMENT	ALT_BIOS_CTRL_ROM

	PUBLIC	check_alt_bios_rst_08
	PUBLIC	check_alt_bios_rst_10

	XREF	_rst_08_functions
	XREF	_rst_10_functions

check_alt_bios_rst_08:
	JP	_rst_08_functions

check_alt_bios_rst_10:
	JP	_rst_10_functions


__IFL_EraseInfoPage:


_alt_bios_flag:
	DB	%00

_alt_bios_failure:
	DB	%00

_alt_bios_reserved:
	DS	16



; CLEAN BOOT
; 1. MAIN BIOS LOOKS FOR INFO BYTE
; 2. ITS INDICATES A NEW ALT-BIOS IS READY TO BE STARTED
; 3
; 3.
; MAIN BIOS PROBES FOR ALT VIA ROM MARKER @ 0x01FF00
; 2.
