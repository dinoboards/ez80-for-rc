
; V9958 REGISTERS
VDP_DATA:	equ	$FF98		; VDP data port (VRAM read/write)
VDP_ADDR:	equ	$FF99		; VDP address (write only)
VDP_STAT:	equ	$FF99		; VDP status (read only)
VDP_PALT:	equ	$FF9A		; VDP palette latch (write only)
VDP_REGS:	equ	$FF9B		; VDP register access (write only)

CMD_HMMC	equ	$F0
CMD_HMMV	equ	$C0

.macro	EZ80_DELAY
	RST.L	$18
.endm

.if ENABLE_DELAY=1
.macro	DELAY_1_7US  ; approx 1.7 us @25Mhz CPU
	PUSH	AF
	POP	AF
	PUSH	AF
	POP	AF
	PUSH	AF
	POP	AF
	PUSH	AF
	POP	AF
	NOP
	NOP
.endm

.macro	DELAY_VDP
	EZ80_DELAY ; 5-6us seconds
.endm

.else
.macro	DELAY_1_7US
.endm
.macro	DELAY_VDP
.endm
.endif
