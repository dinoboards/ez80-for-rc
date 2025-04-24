

        INCLUDE "..\config.inc"

	SECTION CODE

	.assume adl=1

	PUBLIC	_usb_dispatch
	XREF	_usb_init
	XREF	_usb_scsi_seek				; aka _usb_storage_seek
	XREF	_usb_get_device_type
	XREF	_usb_scsi_init
	XREF	_usb_scsi_read_capacity
	XREF	_usb_scsi_read
	XREF	_usb_scsi_write
	XREF	_usb_ufi_get_cap
	XREF	_usb_ufi_write
	XREF	_usb_ufi_read
	XREF	_usb_kyb_init
	XREF	_usb_kyb_status
	XREF	_usb_kyb_read
	XREF	_usb_kyb_flush

_usb_dispatch:
	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, usb_scsi_read			; B = 0
	DEC	A
	JR	Z, usb_scsi_write			; B = 1
	DEC	A
	JR	Z, usb_scsi_read_capacity		; B = 2
	SUB	13
	JR	Z, usb_scsi_init			; B = 15

	DEC	A
	JR	Z, usb_ufi_read				; B = 16
	DEC	A
	JR	Z, usb_ufi_write			; B = 17
	DEC	A
	JR	Z, usb_ufi_get_cap			; B = 18

	SUB	14
	JR	Z, usb_kyb_status			; B = 32
	DEC	A
	JR	Z, usb_kyb_read				; B = 33
	DEC	A
	JR	Z, usb_kyb_flush			; B = 34
	SUB	13
	JR	Z, usb_kyb_init				; B = 47

	LD	A, B
	INC	A
	JR	Z, usb_init				; B = 255
	INC	A
	JR	Z, usb_storage_seek			; B = 254
	INC	A
	JR	Z, usb_get_device_type			; B = 253

	LD	A, %FF					; UNKNOWN FUNCTION
	RET.L

; ----------------------------------------------
; SCSI INTERFACE
; ----------------------------------------------

;
; Function B = ?? -- (USB_SCSI_INIT)
;
; Inputs
;  C -> Device index of scsi usb device
;
; Outputs
;  A -> usb_error
;
; marshalls to usb_error usb_scsi_init(const uint16_t dev_index);
usb_scsi_init:
	PUSH	BC
	CALL	_usb_scsi_init
	POP	BC
	RET.L
;
; Function B = ?? -- scsi_read_capacity (USB_SCSI_READ_CAPACITY)
;
; Inputs
;  C -> Device Index
;  uDE -> pointer to a scsi_read_capacity_result to receive result
;
; Outputs
;  A -> usb_error
;
;marshalls to usb_error usb_scsi_read_capacity(const uint16_t dev_index, scsi_read_capacity_result *result);
usb_scsi_read_capacity:
	PUSH	DE
	PUSH	BC
	CALL	_usb_scsi_read_capacity
	POP	BC
	POP	DE
	RET.L
;
; Function B = ?? usb_scsi_read (USB_SCSI_READ)
;
; Inputs
;  C -> device index
;  uDE -> pointer to a buffer to receive 512 bytes for the current LBA
;
; marshalls to usb_error usb_scsi_read(const uint16_t dev_index, uint8_t *const buffer);
usb_scsi_read:
	PUSH	DE
	PUSH	BC
	CALL	_usb_scsi_read
	POP	BC
	POP	DE
	RET.L
;
; Function B = ?? usb_scsi_write
;
; Inputs
;  C -> device index
;  uDE -> pointer to a buffer of 512 bytes to be written at the current LBA
;
; marshalls to usb_error usb_scsi_write(const uint16_t dev_index, uint8_t *const buffer);
usb_scsi_write:
	PUSH	DE
	PUSH	BC
	CALL	_usb_scsi_write
	POP	BC
	POP	DE
	RET.L


; ----------------------------------------------
; UFI INTERFACE
; ----------------------------------------------


; Function B = ?? usb_ufi_read
;
; Inputs
;  C -> device index
;  uDE -> pointer to a buffer to receive 512 bytes for the current LBA
;
; marshalls to usb_error usb_ufi_read(const uint16_t dev_index, uint8_t *const buffer);
;
usb_ufi_read:
	PUSH	DE
	PUSH	BC
	CALL	_usb_ufi_read
	POP	BC
	POP	DE
	RET.L
;
; Function B = ?? usb_ufi_write
;
; Inputs
;  C -> device index
;  uDE -> pointer to a buffer of 512 bytes to be written at the current LBA
;
; marshalls to usb_error usb_ufi_write(const uint16_t dev_index, uint8_t *const buffer);
;
usb_ufi_write:
	PUSH	DE
	PUSH	BC
	CALL	_usb_ufi_write
	POP	BC
	POP	DE
	RET.L
;
; Function B = ?? -- usb_ufi_get_cap
;
; Inputs
;  C -> Device Index
;
; Outputs
;  A -> usb_error
;  E:uHL -> count of sectors (assuming 512 size)
;
;marshalls to uint32_t usb_ufi_get_cap(const uint16_t dev_index);
usb_ufi_get_cap:
	PUSH	BC
	CALL	_usb_ufi_get_cap
	POP	BC
	RET.L


; ----------------------------------------------
; KYB INTERFACE
; ----------------------------------------------


;
; Function B = ?? -- usb_kyb_init
;
; Inputs
;  C -> Device index of usb keyboard
;
; Outputs
;  A -> usb_error
;
; marshalls to void usb_kyb_init(const uint8_t dev_index);
usb_kyb_init:
	PUSH	BC
	CALL	_usb_kyb_init
	POP	BC
	RET.L
;
; Function B = ?? -- usb_kyb_status
;
; Inputs
;  None
;
; Outputs
;  A -> error or pending count
;
; Return a count of the number of key Codes Pending (A) in the keyboard buffer.
; If it is not possible to determine the actual number in the buffer, it is
; acceptable to return 1 to indicate there are key codes available to read and
; 0 if there are none available.
; The value returned in register A is used as both a Status (A) code and the
; return value. Negative values (bit 7 set) indicate an (error) code.
; Otherwise, the return value represents the number of key codes pending.
;
; marshalls to uint8_t usb_kyb_status();
usb_kyb_status:
	CALL	_usb_kyb_status
	RET.L
;
; Function B = ?? -- usb_kyb_read
;
; Inputs
;  None
;
; Outputs
;   A -> Status
;   H -> Keycode
;
; Read the next key data from the keyboard. If a buffer is used, return the next key code in the buffer.
; If no key data is available, this function will wait indefinitely for a key press. The Status (A) is a
; standard usb_error result code.
;
; The ascii Keycode (H) is generally returned as appropriate ASCII values, if possible. Special keys, like
; function keys and arrows, are returned as reserved codes.
;
; marshalls to uint16_t usb_kyb_read()
usb_kyb_read:
	CALL	_usb_kyb_read
	LD	A, H
	RET.L
;
; Function B = ?? -- usb_kyb_flush
;
; Inputs
;  None
;
; Outputs
;   A: Status
;
; Purged all buffered/queued content.
;
; marshalls to uint32_t usb_kyb_flush()
usb_kyb_flush:
	CALL	_usb_kyb_flush
	RET.L





;
;
; Function B = ?? usb_control_transfer
;
; Inputs
;   IY -> cmd_packet
;   IX -> buffer
;   D -> device address
;   E -> max_packet_size
;
; Outputs
;   A -> usb_error
;
; marshall to usb_error usb_control_transfer(const setup_packet *const cmd_packet, void *const buffer, const uint8_t device_address, const uint8_t max_packet_size);

;
; Function B = ?? usb_data_in_transfer
;
; Inputs
;   IX -> buffer
;   HL -> buffer size
;   D -> device address
;   IY -> endpoint
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usb_data_in_transfer(uint8_t *buffer, const uint16_t buffer_size, const uint8_t device_address, endpoint_param *const endpoint);


;
; Function B = ?? usb_data_in_transfer_n
;
; Inputs
;   IX -> buffer
;   HL -> buffer size
;   D -> device address
;   IY -> endpoint
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usb_data_in_transfer_n(uint8_t *buffer, uint8_t *const buffer_size, const uint8_t device_address, endpoint_param *const endpoint);

;
; Function B = ?? usb_data_out_transfer
;
; Inputs
;   IX -> buffer
;   HL -> buffer size
;   D -> device address
;   IY -> endpoint
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usb_data_out_transfer(const uint8_t *buffer, uint16_t buffer_size, const uint8_t device_address, endpoint_param *const endpoint);




;
; Function B = ?? usbtrn_get_descriptor
;
; Inputs
;   IX -> buffer
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usbtrn_get_descriptor(device_descriptor *const buffer);


;
; Function B = ?? usbtrn_get_descriptor2
;
; Inputs
;   IX -> buffer
;   D -> device address
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usbtrn_get_descriptor2(device_descriptor *const buffer, const uint8_t device_address);


;
; Function B = ?? usbtrn_get_config_descriptor
;
; Inputs
;   IX -> buffer
;   C -> config_index
;   HL -> buffer_size
;   D -> device_address
;   E -> max_packet_size
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usbtrn_get_config_descriptor(config_descriptor *const buffer, const uint8_t config_index, const uint8_t buffer_size, const uint8_t device_address, const uint8_t max_packet_size);

;
; Function B = ?? usbtrn_gfull_cfg_desc
;
; Inputs
;   IX -> buffer
;   C -> config_index
;   D -> device address
;   E -> max packet size
;   HL -> buffer size
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usbtrn_gfull_cfg_desc(const uint8_t config_index, const uint8_t device_address, const uint8_t max_packet_size, const uint8_t max_buffer_size, uint8_t *const buffer);


;
; Function B = ?? usbtrn_set_configuration
;
; Inputs
;   D -> device address
;   E -> max packet size
;   C -> configuration
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usbtrn_set_configuration(const uint8_t device_address, const uint8_t max_packet_size, const uint8_t configuration);

;
; Function B = ?? usbtrn_set_address
;
; Inputs
;   D -> device address
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usbtrn_set_address(const uint8_t device_address);

;
; Function B = ?? usbtrn_clear_endpoint_halt
;
; Inputs
;   D -> device address
;   E -> max packet size
;   C -> endpoint number
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usbtrn_clear_endpoint_halt(const uint8_t endpoint_number, const uint8_t device_address, const uint8_t max_packet_size);





;
; Function B = ?? usb_init
;
; Inputs
;  L -> state initialization.  start with 0 and increment on each failure until success or give up
;   0: Probe for CH376 module.  On initial power will typically need few attempts
;   1: Retrieve version number of CH376 Module
;   2: Probe for connected USB device
;   3..255: Continue to probe for connected USB device
;
; Outputs
;  A -> 0 if success, otherwise indicates a failure
;  L -> State (0...n)
;    0: CH376 module failed to respond.  Retry with state 0
;    1: CH376 module responded. H contains CH376 module version
;    2: H contains count of detected devices..
;    3: H contains count of detected devices..
;
; marshalls to uint16_t usb_init(uint8_t state)
usb_init:
	PUSH	HL
	CALL	_usb_init
	POP	AF
	XOR	A
	RET.L
;
; Function B = ?? usb_storage_seek
;
; Inputs
;  C -> Device Index
;  L:uDE -> 32 bit LBA number
;
; Outputs
;   A -> usb_error
;
; marshalls to usb_error usb_scsi_seek(const uint16_t dev_index, const uint32_t lba) {
usb_storage_seek:
	PUSH	HL
	PUSH	DE
	PUSH	BC
	CALL	_usb_scsi_seek
	POP	BC
	POP	DE
	POP	HL
	RET.L
;
; Function B = ?? usb_get_device_type
;
; Inputs
;  C -> Device Index
;
; Outputs
;   A -> device_type (-1 is no device at dev_index)
;
;marshalls to usb_device_type usb_get_device_type(const uint16_t dev_index)
usb_get_device_type:
	PUSH	BC
	CALL	_usb_get_device_type
	POP	BC
	RET.L
