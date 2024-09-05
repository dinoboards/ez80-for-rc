
        INCLUDE "..\..\config.inc"

        DEFINE	FLASH_RAM_CODE, SPACE = RAM
        SEGMENT	FLASH_RAM_CODE

	SECTION CODE

	XREF	__IFL_EraseInfoPage
	XREF	__IFL_ProgramInfoPage
	XREF	__IFL_ReadInfoPage

	XREF	__FLASH_ROM_CODE
	XREF	__FLASH_RAM_CODE
	XREF	__LENGTH_FLASH_CODE


	.assume adl=1

	PUBLIC	_IFL_EraseInfoPage
_IFL_EraseInfoPage:
	CALL	_install_into_ram
	JP	__IFL_EraseInfoPage

	PUBLIC	_IFL_ProgramInfoPage
_IFL_ProgramInfoPage:
	CALL	_install_into_ram
	JP	__IFL_ProgramInfoPage

	PUBLIC	_IFL_ReadInfoPage
_IFL_ReadInfoPage:
	CALL	_install_into_ram
	JP	__IFL_ReadInfoPage

_install_into_ram:
	PUSH	BC
	PUSH	DE
	PUSH	HL
	LD	HL, __FLASH_ROM_CODE
	LD	DE, __FLASH_RAM_CODE
	LD	BC, __LENGTH_FLASH_CODE
	LDIR
	POP	HL
	POP	DE
	POP	BC
	RET

	END
