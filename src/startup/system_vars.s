        INCLUDE "..\config.inc"

	SEGMENT	BSS

	.assume adl=1

	PUBLIC	_rtc_enabled
	PUBLIC	_cpu_freq_calculated

_rtc_enabled:
	DB	0

_cpu_freq_calculated:
	DS	4

