
	; XDEF	_errno
	XDEF	__c_startup
        XREF	_main

        XREF	__low_bss			; Low address of bss segment
        XREF	__len_bss			; Length of bss segment

        XREF	__low_data			; Address of initialized data section
        XREF	__low_romdata			; Addr of initialized data section in ROM
        XREF	__len_data			; Length of initialized data section

        XREF	__copy_code_to_ram
        XREF	__len_code
        XREF	__low_code
        XREF	__low_romcode

	XREF	__ALT_FIRMWARE_CTRL_ROM
	XREF	__ALT_FIRMWARE_CTRL_RAM
	XREF	__LENGTH_ALT_BIOS_CTRL

	XREF	__FLASH_ROM_CODE
	XREF	__FLASH_RAM_CODE
	XREF	__LENGTH_FLASH_CODE

;*****************************************************************************
; Startup code
        DEFINE	.STARTUP, SPACE = ROM
        SEGMENT	.STARTUP
        .ASSUME ADL=1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initializes the C environment
__c_startup:
_c_int0:
	;--------------------------------------------------
	; Clear the uninitialized data section
	ld	bc, __len_bss			; Check for non-zero length
	ld	a, __len_bss >> 16
	or	a, c
	or	a, b
	jr	z, _c_bss_done			; BSS is zero-length ...
	xor	a, a
	ld	(__low_bss), a
	sbc	hl, hl	 			; hl = 0
	dec	bc	    			; 1st byte's taken care of
	sbc	hl, bc
	jr	z, _c_bss_done			; Just 1 byte ...
	ld	hl, __low_bss			; reset hl
	ld	de, __low_bss + 1		; [de] = bss + 1
	ldir					; Clear this section
_c_bss_done:

	;--------------------------------------------------
	; Copy the initialized data section
	ld	bc, __len_data			; [bc] = data length
	ld	a, __len_data >> 16		; Check for non-zero length
	or	a, c
	or	a, b
	jr	z, _c_data_done			; __len_data is zero-length ...
	ld	hl, __low_romdata		; [hl] = data_copy
	ld	de, __low_data			; [de] = data
	ldir			 		; Copy the data section
_c_data_done:

	;--------------------------------------------------
	; Copy CODE (which may be in FLASH) to RAM if the
	; copy_code_to_ram symbol is set in the link control file
	ld	a, __copy_code_to_ram
	or	a, a
	jr	z, _copy_code_to_ram_done
	ld	bc, __len_code			; [bc] = code length
	ld	a, __len_code >> 16		; Check for non-zero length
	or	a, c
	or	a, b
	jr	z, _copy_code_to_ram_done	; __len_code is zero-length ...
	ld	hl, __low_romcode		; [hl] = code_copy
	ld	de, __low_code			; [de] = code
	ldir					; Copy the code section
_copy_code_to_ram_done:
	; C environment created, continue with the initialization process

; copy ALT_BIOS_CTRL code to top of RAM
	LD	HL, __ALT_FIRMWARE_CTRL_ROM
	LD	DE, __ALT_FIRMWARE_CTRL_RAM
	LD	BC, __LENGTH_ALT_BIOS_CTRL
	LDIR

; copy FLASH INFO helper code
	LD	HL, __FLASH_ROM_CODE
	LD	DE, __FLASH_RAM_CODE
	LD	BC, __LENGTH_FLASH_CODE
	LDIR

	ret

;*****************************************************************************
; Define global system var _errno. Used by floating point libraries
; 	SEGMENT	DATA
; _errno:
; 	DS	3				; extern int _errno

        END
