;
;

; ALT_FIRMWARE_STAT Byte Description
; ----------------------------------
; The ALT_FIRMWARE_STAT byte is used to control the booting process of the alternate firmware.
; It allows the system to determine if the alt-firmware can be booted, has booted successfully,
; or should not be booted, thereby maintaining the main firmware as a fallback.
;
; The alt-firmware is stored in the top 64k of the flash memory (0x01FFFF). If the alt-firmware fails to boot
; properly, the main firmware will take over and stop attempting to run the failing alt-firmware.
; This is determined by reading the value of the ALT_FIRMWARE_STAT byte.
;
; The alt-firmware is responsible for clearing specific bits in this flag to indicate its booting and working.
; The bits are defined as follows:
;
; Bit 0: Alt-firmware attempting
;	- 1: Alt-firmware has not been attempted to start
;	- 0: Alt-firmware has been attempted to start (or has started and is working).
;
; Bit 1: Alt-firmware Version Exchanged
;	- 1: if the alt-firmware's RST 10 SYSUTL_VER_EXCHANGE has not been called
;	- 0: if the alt-firmware's RST 10 SYSUTL_VER_EXCHANGE has been called

; Bit 2: External BIOS code has explicitly called RST 10 util function SYSUTL_BOOT_OK
;        - 1: Alt-firmware has not started or failed to start.
;        - 0: Alt-firmware has been marked by BIOS as good.
;
; Initial State:
; - On initial flash, ALT_FIRMWARE_STAT is set to 0xFF (all bits set to 1).
;
; Status Values:
; - 0xFF (~0x00): New alt-firmware installed but yet to be attempted.
; - 0xFE (~0x01): Alt-firmware attempted and failed.
; - 0xFC (~0x02): Alt-firmware attempted and succeeded.
;
; The main firmware will read the ALT_FIRMWARE_STAT byte to decide whether to attempt booting
; the alt-firmware or to continue with the main firmware.

	include "..\config.inc"

        DEFINE	ALT_FIRMWARE_CTRL_ROM, SPACE = ROM
        SEGMENT	ALT_FIRMWARE_CTRL_ROM

        DEFINE	ALT_FIRMWARE_CTRL_RAM, SPACE = RAM, ORG=%02FF00
        SEGMENT	ALT_FIRMWARE_CTRL_RAM

	.assume adl=1

	SEGMENT	CODE

IFDEF RC2014_ALT_FIRMWARE
	ORG	ALT_FIRMWARE_INFO

MARKER:
	DB	"RC2014-ALT-FIRMWARE", %00

	ORG	ALT_FIRMWARE_STAT
	DB	%FF
ELSE

	PUBLIC	_attempt_alt_firmware

_attempt_alt_firmware:
	LD	HL, ALT_FIRMWARE_INFO
	LD	DE, STRING_TO_CHECK 			; Load the address of the string to check against into DE
	LD	B, 6               			; Length of the string "RC2014" is 6

compare_loop:
	LD	A, (DE)					; Load the expected byte
	CP	(HL)					; Compare it with byte withing alt flash space
	RET	NZ					; return NZ if not equal - and resume with main bios startup
	INC	HL
	INC	DE
	DJNZ	compare_loop

; if *ALT_FIRMWARE_STAT == ~0x00 (0xFF), then new alt-firmware installed, but yet to be attempted
; if *ALT_FIRMWARE_STAT == ~0x01 (0xFE), then alt-firmware attempted and failed
; if *ALT_FIRMWARE_STAT == ~0x02 (0xFC), then alt-firmware attempted and succeeded

	LD	A, (ALT_FIRMWARE_STAT)
	CP	~%00
	JR	Z, attempt_alt_firmare

	CP	~%01					; alt-firmware has been attempted and failed
	RET	Z					; so return and continue with main bios

	CP	~%02					; alt-firmware has been attempted and succeeded
	JP	Z, ALT_FIRMWARE_BASE			; so jump to alt-firmware

	; unknown state, so continue with main bios
	RET

attempt_alt_firmare:
	; a new alt-firmware has been installed
	; so lets mark it as attempt 1 and start alt-firmware
	ENABLE_FLASH_WRITES ~%80

	LD	A, ~%01
	LD	(ALT_FIRMWARE_STAT), A

	DISABLE_FLASH_WRITES

	JP	ALT_FIRMWARE_BASE

STRING_TO_CHECK:
	DB	"RC2014"

ENDIF

	SEGMENT	ALT_FIRMWARE_CTRL_ROM

	PUBLIC	FIRMWARE_RAM_RESERVED
FIRMWARE_RAM_RESERVED:
_firmware_ram_reseved:

	XREF	_rst_08_functions
	PUBLIC	check_alt_firmware_rst_08
check_alt_firmware_rst_08:
	JP	_rst_08_functions

	XREF	_rst_10_functions
	PUBLIC	check_alt_firmware_rst_10
check_alt_firmware_rst_10:
	JP	_rst_10_functions

	XREF	_system_timer_isr
	PUBLIC	_system_timer_isr_hook
_system_timer_isr_hook:
	JP	_system_timer_isr

	XREF	_marshall_isr
	PUBLIC	_marshall_isr_hook
_marshall_isr_hook:
	JP	_marshall_isr

	XREF	_uart0_receive_isr
	PUBLIC	_uart0_receive_isr_hook
_uart0_receive_isr_hook:
	JP	_uart0_receive_isr

	PUBLIC	_default_mi_handler_hook
_default_mi_handler_hook:					; if alt-firmware wants to handle a default interrupt
	POP	AF					; it can change this to be a 'jump' to its own handler
	EI						; a will indicate the index of the interrupt
	RETI.L
	DB	0, 0, 0, 0



_reserved:
	DB	0, 0, 0, 0, 0, 0, 0, 0
	DB	0, 0, 0, 0, 0, 0, 0, 0


