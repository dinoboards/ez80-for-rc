
	.assume	adl=1

	SECTION	DATA

	PUBLIC	_VDP_IO_DATA, _VDP_IO_ADDR, _VDP_IO_STAT, _VDP_IO_PALT, _VDP_IO_REGS

; Default the VDP register values to the MSX default standard
; V9958 REGISTERS
_VDP_IO_DATA:
	dw24	%FF98		; VDP data port (VRAM read/write)
_VDP_IO_ADDR:
	dw24	%FF99		; VDP address (write only)
_VDP_IO_STAT:
	dw24	%FF99		; VDP status (read only)
_VDP_IO_PALT:
	dw24	%FF9A		; VDP palette latch (write only)
_VDP_IO_REGS:
	dw24	%FF9B		; VDP register access (write only)
