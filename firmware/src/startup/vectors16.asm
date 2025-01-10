	include "../config.inc"

        DEFINE	ZFL_nINT_Flash, SPACE = RAM

; Reset and all RST nn's
;  1. diaable interrupts
;  2. clear mixed memory mode (MADL) flag
;  3. jump to initialization procedure with jp.lil to set ADL
	DEFINE	.RESET, SPACE = ROM
	SEGMENT	.RESET

	XREF	__init
	XREF	__low_rom
	XDEF	_reset
	XREF	firmware_rst_08_hook
	XREF	firmware_rst_10_hook
	XREF	firmware_rst_18_hook
	XREF	_default_mi_handler_hook
	XREF	_marshall_isr_hook


_reset:
_rst0:
	di
	stmix
	jp.lil	__init

	ORG	ROM_BASE+%08
_rst8:
	jp.lil	firmware_rst_08_hook

	org	ROM_BASE+%10
_rst10:
	jp.lil	firmware_rst_10_hook

	org	ROM_BASE+%18
_rst18:
	jp.lil	firmware_rst_18_hook

	org	ROM_BASE+%20
_rst20:
	di
	stmix
	jp.lil	__init
_rst28:
	di
	stmix
	jp.lil	__init
_rst30:
	di
	stmix
	jp.lil	__init
_rst38:
	di
	stmix
	jp.lil	__init
	ds %26
_nmi:
	jp.lil	__default_nmi_handler


;*****************************************************************************
; Startup code
	DEFINE .STARTUP, SPACE = ROM
	SEGMENT .STARTUP
	.ASSUME ADL=1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Default Non-Maskable Interrupt handler
__default_nmi_handler:
	RETN.L

DEFAULT_MI_HANLDER: MACRO id
_default_mi_&id&_handler:
	PUSH	AF
	LD	A, %&id
	JP	_default_mi_handler_hook
	ENDMAC

DEFAULT_MI_HANLDER 00
DEFAULT_MI_HANLDER 01
DEFAULT_MI_HANLDER 02
DEFAULT_MI_HANLDER 03
DEFAULT_MI_HANLDER 04
DEFAULT_MI_HANLDER 05
DEFAULT_MI_HANLDER 07
DEFAULT_MI_HANLDER 08
DEFAULT_MI_HANLDER 09
DEFAULT_MI_HANLDER 0A
DEFAULT_MI_HANLDER 0B
DEFAULT_MI_HANLDER 0D
DEFAULT_MI_HANLDER 0E
DEFAULT_MI_HANLDER 0F
DEFAULT_MI_HANLDER 10
DEFAULT_MI_HANLDER 11
DEFAULT_MI_HANLDER 12
DEFAULT_MI_HANLDER 13
DEFAULT_MI_HANLDER 14
DEFAULT_MI_HANLDER 15
DEFAULT_MI_HANLDER 16
DEFAULT_MI_HANLDER 17
DEFAULT_MI_HANLDER 18
DEFAULT_MI_HANLDER 19
DEFAULT_MI_HANLDER 1A
DEFAULT_MI_HANLDER 1B
DEFAULT_MI_HANLDER 1D
DEFAULT_MI_HANLDER 1E
DEFAULT_MI_HANLDER 1F
DEFAULT_MI_HANLDER 20
DEFAULT_MI_HANLDER 21
DEFAULT_MI_HANLDER 22
DEFAULT_MI_HANLDER 23
DEFAULT_MI_HANLDER 24
DEFAULT_MI_HANLDER 25
DEFAULT_MI_HANLDER 26
DEFAULT_MI_HANLDER 27
DEFAULT_MI_HANLDER 28
DEFAULT_MI_HANLDER 29
DEFAULT_MI_HANLDER 2A
DEFAULT_MI_HANLDER 2B
DEFAULT_MI_HANLDER 2C
DEFAULT_MI_HANLDER 2D
DEFAULT_MI_HANLDER 2E
DEFAULT_MI_HANLDER 2F

_system_timer_isr_rom_hook:
	JP	_system_timer_isr_hook

_uart0_receive_isr_rom_hook:
	JP	_uart0_receive_isr_hook

_marshall_isr_rom_hook:
	JP	_marshall_isr_hook


;*****************************************************************************
; Interrupt Vector Table
;  - this segment must be aligned on a 256 byte boundary anywhere below
;    the 64K byte boundry
;  - each 2-byte entry is a 2-byte vector address
	DEFINE .IVECTS, SPACE = ROM, ALIGN = 100h
	SEGMENT .IVECTS

	PUBLIC	__vector_table
	EXTERN	_system_timer_isr_hook
	EXTERN _uart0_receive_isr_hook

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
__vector_table:
	dw	_default_mi_00_handler		; 00H - unused
	dw	_default_mi_01_handler		; 02H - unused
	dw	_default_mi_02_handler		; 04H - unused
	dw	_default_mi_03_handler		; 06H - unused
	dw	_default_mi_04_handler		; 08H - Flash
	dw	_default_mi_05_handler		; 0AH - PRT 0
	dw	_system_timer_isr_rom_hook	; 0CH - PRT 1
	dw	_default_mi_07_handler		; 0EH - PRT 2
	dw	_default_mi_08_handler		; 10H - PRT 3
	dw	_default_mi_09_handler		; 12H - PRT 4
	dw	_default_mi_0A_handler		; 14H - PRT 5
	dw	_default_mi_0B_handler		; 16H - RTC
	dw	_uart0_receive_isr_rom_hook	; 18H - UART 0
	dw	_default_mi_0D_handler		; 1AH - UART 1
	dw	_default_mi_0E_handler		; 1CH - I2C
	dw	_default_mi_0F_handler		; 1EH - SPI
	dw	_default_mi_10_handler		; 20H - unused
	dw	_default_mi_11_handler		; 22H - unused
	dw	_default_mi_12_handler		; 24H - unused
	dw	_default_mi_13_handler		; 26H - unused
	dw	_default_mi_14_handler		; 28H - unused
	dw	_default_mi_15_handler		; 2AH - unused
	dw	_default_mi_16_handler		; 2CH - unused
	dw	_default_mi_17_handler		; 2EH - unused
	dw	_default_mi_18_handler		; 30H - Port B0
	dw	_default_mi_19_handler		; 32H - Port B1
	dw	_default_mi_1A_handler		; 34H - Port B2
	dw	_default_mi_1B_handler		; 36H - Port B3
	dw	_marshall_isr_rom_hook		; 38H - Port B4
	dw	_default_mi_1D_handler		; 3AH - Port B5
	dw	_default_mi_1E_handler		; 3CH - Port B6
	dw	_default_mi_1F_handler		; 3EH - Port B7
	dw	_default_mi_20_handler		; 40H - Port C0
	dw	_default_mi_21_handler		; 42H - Port C1
	dw	_default_mi_22_handler		; 44H - Port C2
	dw	_default_mi_23_handler		; 46H - Port C3
	dw	_default_mi_24_handler		; 48H - Port C4
	dw	_default_mi_25_handler		; 4AH - Port C5
	dw	_default_mi_26_handler		; 4CH - Port C6
	dw	_default_mi_27_handler		; 4EH - Port C7
	dw	_default_mi_28_handler		; 50H - Port D0
	dw	_default_mi_29_handler		; 52H - Port D1
	dw	_default_mi_2A_handler		; 54H - Port D2
	dw	_default_mi_2B_handler		; 56H - Port D3
	dw	_default_mi_2C_handler		; 58H - Port D4
	dw	_default_mi_2D_handler		; 5AH - Port D5
	dw	_default_mi_2E_handler		; 5CH - Port D6
	dw	_default_mi_2F_handler		; 5EH - Port D7

	END
