
	include ".\config.inc"

	SECTION CODE

	.assume adl=1

; EZ80_DELAY (RST %18)
; Wait 5 to 6 us (approx) for the eZ80
; indepdent of the clock speed
;
; Input
;  None
;
; Output
;
	PUBLIC	check_alt_firmware_rst_18
check_alt_firmware_rst_18:
	; //timer 0 has reload value that will equate to about 5-6us
	; so set it off, and wait.

	LD	A, TMR_ENABLED | TMR_SINGLE | TMR_RST_EN | TMR_CLK_DIV_4
	OUT0	(TMR0_CTL), A

wait:
	IN0	A, (TMR0_DR_L)
	JR	NZ, wait
	RET.L
