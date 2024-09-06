
        INCLUDE "..\..\config.inc"

        DEFINE	FLASH_RAM_CODE, SPACE = RAM
        SEGMENT	FLASH_RAM_CODE

	SECTION CODE

FLASH_RAM_CONENT_NONE	EQU	%00
FLASH_RAM_CONENT_IFL	EQU	%02

	XREF	__IFL_EraseInfoPage
	XREF	__IFL_ProgramInfoPage
	XREF	__IFL_ReadInfoPage
	XREF	__IFL_WriteInfoByte

	XREF	__FLASH_ROM_CODE
	XREF	__FLASH_RAM_CODE
	XREF	__LENGTH_FLASH_CODE

	XREF	_flash_ram_content

	.assume adl=1

	PUBLIC	_IFL_EraseInfoPage
_IFL_EraseInfoPage:
	CALL	_install_ifl_into_ram
	JP	__IFL_EraseInfoPage

	PUBLIC	_IFL_ProgramInfoPage
_IFL_ProgramInfoPage:
	CALL	_install_ifl_into_ram
	JP	__IFL_ProgramInfoPage

	PUBLIC	_IFL_ReadInfoPage
_IFL_ReadInfoPage:
	CALL	_install_ifl_into_ram
	JP	__IFL_ReadInfoPage

	PUBLIC _IFL_WriteInfoByte
_IFL_WriteInfoByte:
	CALL	_install_ifl_into_ram
	JP	__IFL_WriteInfoByte

_install_ifl_into_ram:
	LD	A, (_flash_ram_content)
	CP	FLASH_RAM_CONENT_IFL
	RET	Z
	LD	A, FLASH_RAM_CONENT_IFL
	LD	(_flash_ram_content), A
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
