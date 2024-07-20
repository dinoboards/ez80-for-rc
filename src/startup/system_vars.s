        INCLUDE "..\config.inc"

	SEGMENT	BSS

	.assume adl=1

	PUBLIC	_rtc_enabled
	PUBLIC	_cpu_freq_calculated
	PUBLIC	_platform_description
	PUBLIC	_romwbw_version
	PUBLIC	_romwbw_version_major
	PUBLIC	_romwbw_version_minor
	PUBLIC	_romwbw_version_patch
	PUBLIC	_romwbw_version_revision
	PUBLIC	_romwbw_cpu_frequency
	PUBLIC	_romwbw_platform_code
	PUBLIC	_romwbw_memory_manager_code
	PUBLIC	_romwbw_ramsize
	PUBLIC	_romwbw_romsize

_rtc_enabled:
	DB	0

_cpu_freq_calculated:
	DS	4

_platform_description:
_romwbw_version:
_romwbw_version_major:
	DS	1
_romwbw_version_minor:
	DS	1
_romwbw_version_patch:
	DS	1
_romwbw_version_revision:
	DS	1
_romwbw_cpu_frequency:
	DS	4
_romwbw_platform_code:
	DS	1
_romwbw_memory_manager_code:
	DS	1
_romwbw_ramsize:
	DS	2
_romwbw_romsize:
	DS	2
_romwbw_reserved:
	DS	2

