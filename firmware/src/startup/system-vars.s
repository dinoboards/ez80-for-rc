        INCLUDE "..\config.inc"

	SEGMENT	BSS

	.assume adl=1

	PUBLIC	_rtc_enabled
	PUBLIC	_cpu_freq_calculated

	PUBLIC	_mem_bus_timings
	PUBLIC	_mem_bus_mode_and_timing
	PUBLIC	_mem0_bus_mode_and_timing
	PUBLIC	_mem1_bus_mode_and_timing
	PUBLIC	_io_bus_mode_and_timing

	PUBLIC	_system_ticks
	PUBLIC	_ticks_frequency
	PUBLIC	_ticks_per_10_us

	PUBLIC	_line_control
	PUBLIC	_baud_rate

	PUBLIC	_i2_clk_divider
	PUBLIC	_i2c_bus_freq

	PUBLIC	usb_tick_toggle

_rtc_enabled:
	DB	0

_system_ticks:
	DS	3

_baud_rate:
	DS	3

; I2C clock divider struct (scalar and exponent)
_i2_clk_divider:
	DS	2

; I2C bus frequency
_i2c_bus_freq:
	DS	3

;
; line control bits:
;
; {0:1} = Parity    (00 -> NONE, 01 -> NONE, 10 -> ODD, 11 -> EVEN)
; {2}   = Stop Bits (0 -> 1, 1 -> 2)
; {3:4} = Data Bits (00 -> 5, 01 -> 6, 10 -> 7, 11 -> 8)
; {5:5} = Hardware Flow Control CTS (0 -> OFF, 1 -> ON)
;
_line_control:
	DS	1

_mem_bus_timings:
_mem_bus_mode_and_timing: ; BIT 7 = 0 -> ez80, 1 -> z80, BITS 0 to 2 = cycles or wait state
	DS	1

; Memory timing for CS0
_mem0_bus_mode_and_timing: ; BIT 7 = 0 -> ez80, 1 -> z80, BITS 0 to 2 = cycles or wait state
	DS	1

; Memory timing for CS1
_mem1_bus_mode_and_timing: ; BIT 7 = 0 -> ez80, 1 -> z80, BITS 0 to 2 = cycles or wait state
	DS	1

_io_bus_mode_and_timing: ; BIT 7 = 0 -> ez80, 1 -> z80, BITS 0 to 2 = cycles or wait state
	DS	1


	SECTION	DATA

_cpu_freq_calculated:
	DL	CPU_CLK_FREQ

_ticks_frequency:
	DB	DEFAULT_TICKFREQ

_ticks_per_10_us:
	DB	CPU_CLK_FREQ/16/100000

usb_tick_toggle:
	DB	0

	END
