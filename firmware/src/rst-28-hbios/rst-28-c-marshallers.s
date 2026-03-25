	include "..\config.inc"
	include "..\rst-28-constants.inc"

	section	CODE
	.assume	adl=1

	xref	firmware_rst_28_hook

	PUBLIC	_diodevice_getstatus
; uint8_t diodevice_getstatus(uint8_t);
_diodevice_getstatus:
	PUSH	IX					; save IX in case BF_DIODEVICE modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_DIODEVICE
	CALL.IL	firmware_rst_28_hook
	POP	IX
	RET						; return status in A

	PUBLIC	_diodevice_getdriver
; uint8_t diodevice_getdevice(uint8_t);
_diodevice_getdriver
	PUSH	IX					; save IX in case BF_DIODEVICE modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_DIODEVICE
	CALL.IL	firmware_rst_28_hook
	LD	A, D
	POP	IX
	RET						; return status in A

	PUBLIC	_diodevice_getnumber
; uint8_t diodevice_getnumber(uint8_t);
_diodevice_getnumber
	PUSH	IX					; save IX in case BF_DIODEVICE modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_DIODEVICE
	CALL.IL	firmware_rst_28_hook
	LD	A, E
	POP	IX
	RET						; return status in A

	PUBLIC	_diodevice_getattributes
; uint8_t diodevice_getattributes(uint8_t);
_diodevice_getattributes
	PUSH	IX					; save IX in case BF_DIODEVICE modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_DIODEVICE
	CALL.IL	firmware_rst_28_hook
	LD	A, C
	POP	IX
	RET						; return status in A


	PUBLIC	_dioseek
	; uint8_t dioseek(const uint8_t disk_unit, const uint24_t lba) {
_dioseek:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	B, BF_DIOSEEK
	LD	C, (IX+6) 				; diskunit
	LD	HL, (IX+9) 				; lba
	CALL.IL	firmware_rst_28_hook

	POP	IX
	RET

	PUBLIC	_dioread
	; uint16_t dioread(const uint8_t disk_unit, uint8_t sector_count, const uint8_t *buffer)
_dioread:
	PUSH	IX
	LD	IX,0
	ADD	IX,SP

	LD	B, BF_DIOREAD
	LD	C, (IX+6) 				; diskunit
	LD	E, (IX+9) 				; sector_count
	LD	HL, (IX+12)				; buffer
	CALL.IL	firmware_rst_28_hook

	LD	L, A					; status in low byte
	LD	H, E					; sectors read in high byte

	POP	IX
	RET

	PUBLIC	_diowrite
	; uint16_t diowrite(const uint8_t disk_unit, uint8_t sector_count, const uint8_t *buffer)
_diowrite:
	PUSH	IX
	LD	IX,0
	ADD	IX,SP

	LD	B, BF_DIOWRITE
	LD	C, (IX+6) 				; diskunit
	LD	E, (IX+9) 				; sector_count
	LD	HL, (IX+12)				; buffer
	CALL.IL	firmware_rst_28_hook

	LD	L, A					; status in low byte
	LD	H, E					; sectors written in high byte

	POP	IX
	RET

	PUBLIC	_diomedia
	; uint16_t diomedia(const uint8_t disk_unit);
_diomedia:
	PUSH	IX
	LD	IX,0
	ADD	IX,SP

	LD	B, BF_DIOMEDIA
	LD	C, (IX+6) 				; diskunit
	LD	E, (IX+9) 				; flags
	CALL.IL	firmware_rst_28_hook

	LD	L, A					; status in low byte
	LD	H, E					; media id in high byte

	POP	IX
	RET




	PUBLIC	_diocapacity_get_sectors
; uint24_t diocapacity_get_sectors(uint8_t);
_diocapacity_get_sectors
	PUSH	IX					; save IX in case BF_DIODEVICE modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_DIOCAP
	CALL.IL	firmware_rst_28_hook
	POP	IX
	RET
