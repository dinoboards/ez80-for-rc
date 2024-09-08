; -----------------------------------------------------------------------------
; Dual Firmware Boot Process
; -----------------------------------------------------------------------------
; This assembly file implements the logic to manage a dual firmware boot process.
; The main firmware will attempt to run the alternate firmware up to 3 times,
; tracking unsuccessful boots in the flash info page (ALT_FIRMWARE_BOOT_INFO).
;
; The process is as follows:
;
; 1. First check the main firmware performs is to see if the special marker at 0x01FF00 is set to "RC2014"
;   - If it is not, then no alt firmware is present, so continue with the main bios startup
;
; 2. Next the main bios inspect the flash infor bytes (0xFE...0xFF) and determine the current status of the alt-firmware
;
; if ALT_FIRMWARE_STAT == 0x5A and ALT_FIRMWARE_BOOT_COUNT == 0, then a new image has been flash and has yet to be booted into.
;
; thereafter, on each reboot of the platform, the main firmware will continue to increment the ALT_FIRMWARE_BOOT_COUNT
; if the count reaches 3, then the alt-firmware is considered bad and the main firmware will consider the alt-firmware as bas and
; continue with the its startup
;
; if the bios running under the alt-firmware invoks the RST %10 function SYSUTL_BOOT_VERIFIED, then a value of 0x2F is written to the
; to the ALT_FIRMWARE_BOOT_INFO byte, indicating that the alt-firmware has successfully booted and is considered good and will always henceforth
; be booted into.
;
; -----------------------------------------------------------------------------



;
; -----------------------------------------------------------------------------
; Dual Firmware Boot Process
; -----------------------------------------------------------------------------
; This assembly file implements the logic to manage a dual firmware boot process.  The main firmware will attempt to
; run an alternate firmware up to 3 times, tracking any unsuccessful boots in the flash info page
; at ALT_FIRMWARE_BOOT_INFO.
;
; The main firmware is flashed via a programmer and resides within the address range 0x000000 to 0x00FFFF.
; It is always booted first due to the way the eZ80 operates. If an alternate firmware is detected and considered good,
; the main firmware will switch over to it.  Flashing a new main firmware will also erase the alternate firmware.
;
; The alternate firmware is flashed via the helper application FWUPDATE.COM and resides at 0x010000 to 0x01FFFF.
; The main firmware checks that the special marker at 0x01FF00 is set to "RC2014" to determine if the alternate
; firmware is present. If the marker is not found, the main firmware continues with its startup process.
;
; The dual firmware boot process is as follows:
;
; 1. The main firmware first checks if the special marker at 0x01FF00 is set to "RC2014".
;
; 2.  If it is not, then no alternate firmware is present, so dual boot is aborted, and the main firmware does
;     its normal boot up.
;
; 2. Otherwise, the main firmware will inspect the flash info bytes (ALT_FIRMWARE_STAT & ALT_FIRMWARE_BOOT_COUNT
;    @ 0xFE...0xFF) to determine the current status of the alternate firmware.
;
; 3. If ALT_FIRMWARE_STAT == 0x5A and ALT_FIRMWARE_BOOT_COUNT == 0 then this indicates a new image has been flashed
;    but has yet to be booted into.  For this case, the main firmware increment ALT_FIRMWARE_BOOT_COUNT and then
;    switch over and boots into the alternate firmware.
;
; 4. If ALT_FIRMWARE_STAT == 0x2F then this indicates that the alternate firmware has successfully booted and is
;    considered good.  The main firmware will continue to switch over and boot into the alternative firmware.
;
; 4. If ALT_FIRMWARE_STAT == 0x5A and ALT_FIRMWARE_BOOT_COUNT between 0 and 3, then this indicates previous attempts
;    to boot the alternative firmware has failed.  The main firmware will continue to increment the count and attempt
;    to boot the alternative firmware.
;
; 5. If the count reaches 3, the alternate firmware is permanently considered bad and the main firmware will continue
;    with its boot up process.
;
; If the BIOS running under the alternate firmware invokes the RST %10 function SYSUTL_BOOT_VERIFIED, a value of 0x2F
; will be written to the ALT_FIRMWARE_BOOT_INFO byte, indicating that the alternate firmware has successfully booted
; and is considered good. Henceforth, the alternate firmware will always be booted into.
;

; -----------------------------------------------------------------------------
; Interrupt and RST Handler Delegation
; -----------------------------------------------------------------------------
; The eZ80 processor requires that interrupt and RST vectors be located within the first 64K address range.
; To enable the alternative firmware to be able to have its on handlers, the main firmware must delegate via RAM jump tables.
;
; When the main firmware is active, the in RAM jump tables are redirected back to handlers within the main firmware.
;
; When the alternate firmware is booted, it will update the RAM jump tables to point to its own handlers.
;
; The alternate firmware cannot configure the interrupt vector table address via the I register, as the eZ80 only supports
; having the interrupt vector table within the first 64K address range.
; Likewise, the RST.L handlers must also be from address 0x000000, hence the need for the main BIOS to delegate these to the appropriate
; RAM jump table addresses.
;
; -----------------------------------------------------------------------------

	include "..\config.inc"

	DEFINE	ALT_FIRMWARE_CTRL_ROM, SPACE = ROM
	SEGMENT	ALT_FIRMWARE_CTRL_ROM

	DEFINE	ALT_FIRMWARE_CTRL_RAM, SPACE = RAM, ORG=%02FF00
	SEGMENT	ALT_FIRMWARE_CTRL_RAM

	XREF	_IFL_ReadInfoWord24
	XREF	_IFL_WriteInfoByte

ALT_FIRMWARE_BOOT_INFO	EQU	%FE
ALT_FIRMWARE_STAT 	EQU	1			; this byte is set to 0x5A by fwupdate on firmware updating, otherwise it should be 0xFF
ALT_FIRMWARE_BOOT_COUNT	EQU	0			;

	.assume adl=1

	SEGMENT	CODE

IFDEF RC2014_ALT_FIRMWARE
	PUBLIC	_ez80_exchange_version_verified
_ez80_exchange_version_verified:

	LD	HL, ALT_FIRMWARE_BOOT_INFO
	PUSH	HL
	CALL	_IFL_ReadInfoWord24
	POP	AF
	; HL SHOULD BE THE 3 BYTES (FROM %FE, %FF, 00)

	LD	A, H
	CP	%2F
	RET	Z

	LD	H, %2F
	LD	L, ALT_FIRMWARE_BOOT_INFO + ALT_FIRMWARE_STAT
	PUSH	HL
	CALL	_IFL_WriteInfoByte
	POP	HL

	RET

	ORG	ALT_FIRMWARE_INFO
	DB	"RC2014-ALT-FIRMWARE", %00

ELSE
	ORG	ALT_FIRMWARE_INFO
	DB	"                   ", %00

	SEGMENT	CODE


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

	LD	HL, ALT_FIRMWARE_BOOT_INFO
	PUSH	HL
	CALL	_IFL_ReadInfoWord24
	POP	AF
	; HL SHOULD BE THE 3 BYTES (FROM %FE, %FF, 00)

	LD	A, H
	CP	%2F
	JP	Z, ALT_FIRMWARE_BASE

	CP	%5A					; IF NOT 5A OR 2F, THEN UNKNOWN, SO CONTINUE WITH MAIN BIOS
	RET	NZ

	LD	A, L
	CP	%04
	RET	NC					; IF > 3, THEN CONTINUE WITH MAIN BIOS

	INC	A
	LD	H, A
	LD	L, ALT_FIRMWARE_BOOT_INFO + ALT_FIRMWARE_BOOT_COUNT
	PUSH	HL
	CALL	_IFL_WriteInfoByte
	POP	HL
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


