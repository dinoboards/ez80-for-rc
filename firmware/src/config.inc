; **************
; Main Module Configuration Flags/Settings

; CPU @ 18.432
; WITH BUS-MODE SET TO Z80, BUS-CYCLE OF 3 -> EQUIVALENT TO Z80 @ 6.144MHZ
; NO ADDITIONAL WAIT STATES REQUIRED
;
; CPU @ 20.000
; WITH BUS-MODE SET TO Z80, BUS-CYCLE OF 3 -> EQUIVALENT TO Z80 @ 6.667MHZ
; NO ADDITIONAL WAIT STATES REQUIRED
;
; CPU @ 24.000
; NO ADDITIONAL WAIT STATES REQUIRED
; WITH BUS-MODE SET TO Z80, BUS-CYCLE OF 3 -> EQUIVALENT TO Z80 @ 8MHZ
;
; CPU @ 24.000
; NO ADDITIONAL WAIT STATES REQUIRED
; WITH BUS-MODE SET TO Z80, BUS-CYCLE OF 4 -> EQUIVALENT TO Z80 @ 6MHZ
;
; **************

        INCLUDE "startup\ez80f92.inc"
        INCLUDE ".\romwbw.inc"
        INCLUDE ".\rst-10-constants.inc"

RX_BUFFER_SIZE		.EQU	140
RX_BUFFER_HIGH		.EQU	120
RX_BUFFER_LOW		.EQU	60

DEFAULT_TICKFREQ	.EQU	60

CLK_32			.EQU	32000000
CLK_24			.EQU	24000000
CLK_20			.EQU	20000000
CLK_18_432		.EQU	18432000
CLK_16			.EQU	16000000
CLK_14_7456		.EQU	14745600
CLK_7_3728		.EQU	7372800

CPU_CLK_FREQ		.EQU	CLK_18_432
UART_BPS		.EQU	115200

Z80_ADDR_MBASE		.EQU	%B9

IO_BUS_CYCLES 		.EQU	BMX_BC_5
MEM_BUS_CYCLES 		.EQU	BMX_BC_4