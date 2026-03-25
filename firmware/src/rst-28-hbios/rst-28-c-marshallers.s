	include "..\config.inc"
	include "..\rst-28-constants.inc"

	section	CODE
	.ASSUME	ADL=1

	XREF	firmware_rst_28_hook

	PUBLIC	_ciodevice_getdriver
; uint8_t ciodevice_getdevice(uint8_t);
_ciodevice_getdriver
	PUSH	IX					; save IX in case BF_CIODEVICE modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_CIODEVICE
	CALL.IL	firmware_rst_28_hook
	LD	A, D
	POP	IX
	RET						; return status in A

	PUBLIC	_ciodevice_getnumber
; uint8_t ciodevice_getnumber(uint8_t);
_ciodevice_getnumber
	PUSH	IX					; save IX in case CF_DIODEVICE modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_CIODEVICE
	CALL.IL	firmware_rst_28_hook
	LD	A, E
	POP	IX
	RET						; return status in A

	PUBLIC	_cioquery_get_baud_rate
; uint24_t cioquery_get_baud_rate(uint8_t);
_cioquery_get_baud_rate:
	PUSH	IX					; save IX in case BF_CIOQUERY modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_CIOQUERY
	CALL.IL	firmware_rst_28_hook
	POP	IX
	RET						; return status in A

	PUBLIC	_cioquery_get_line_control
; uint8_t _cioquery_get_line_control(uint8_t);
_cioquery_get_line_control:
	PUSH	IX					; save IX in case BF_CIOQUERY modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_CIOQUERY
	CALL.IL	firmware_rst_28_hook
	LD	A, E
	POP	IX
	RET						; return status in A


	PUBLIC	_ciodevice_getattributes
; uint8_t ciodevice_getattributes(uint8_t);
_ciodevice_getattributes
	PUSH	IX					; save IX in case BF_CIODEVICE modifies it
	LD	IX, 0
	ADD	IX, SP
	LD	C, (IX+6)				; hbios disk_unit
	LD	B, BF_CIODEVICE
	CALL.IL	firmware_rst_28_hook
	LD	A, C
	POP	IX
	RET						; return status in A



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

	PUBLIC	_sysget_ciocount
; uint8_t sysget_ciocount();
_sysget_ciocount:
	LD	BC, +(BF_SYSGET << 8)+BF_SYSGET_CIOCNT
	CALL.IL	firmware_rst_28_hook
	LD	A, E
	RET

	PUBLIC	_sysget_diocount
; uint8_t sysget_diocount();
_sysget_diocount:
	LD	BC, +(BF_SYSGET << 8)+BF_SYSGET_DIOCNT
	CALL.IL	firmware_rst_28_hook
	LD	A, E
	RET
