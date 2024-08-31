
        INCLUDE "..\config.inc"
	INCLUDE "i2c\i2c-constants.inc"

	SECTION CODE

	.assume adl=1

	XREF	__icmpzero
	XREF	_i2c_calc_ccr

	XREF	_i2_clk_divider				; I2C clock divider scalar value
	XREF	_i2c_bus_freq				; I2C bus frequency

	PUBLIC	_i2c_dispatch

I2C_SRESET	.EQU	%12 				; any non-zero value will reset
COUNT_POLL_IFLG	.EQU	30000				; Number of times to poll the IFLG bit

_i2c_dispatch:
	LD	A, B					; SUB FUNCTION CODE
	OR	A					; TEST SUB FUNCTION CODE
	JR	Z, i2c_init				; B = 0
	DEC	A
	JR	Z, i2c_write				; B = 1
	DEC	A
	JR	Z, i2c_read				; B = 2
	DEC	A
	JR	Z, i2c_probe				; B = 3

	LD	A, %FF					; UNKNOWN UART FUNCTION
	RET.L

;
; Function B = 00 -- I2C Init (I2C_INIT)
;
; Initialize the I2C bus.
;
; This function initializes the I2C bus.  The I2C bus is initialized to the default state.  The I2C bus is
; initialized to the following state:
;
; 3.  The I2C bus is configured for 7-bit addressing.
; 4.  The I2C bus is configured for master mode.
;
; Inputs:
;  C	 = 0 -> 100kbps, 1 -> 400kbps
;
; Outputs:
;  A	 = 0 -> Success
;  uHL	 = Actual Frequency (24bit)
;  DE:HL = Actual Frequency (32 bit, E=0)
;  B     = I2C clock divider exponent
;  C     = I2C clock divider scalar value
;
i2c_init:
	CALL	i2c_reinit
	LD	BC, (_i2_clk_divider)
	LD	HL, (_i2c_bus_freq)
	LD	A, (_i2c_bus_freq+2)
	LD	E, A
	XOR	A
	LD	D, A
	RET.L
;
; Function B = 01 -- I2C Write (I2C_WRITE)
;
; Write data to the I2C bus.
;
; This function writes data to the I2C bus.  The I2C bus must be initialized before calling this function.
;
; Inputs:
;  C	= Data Length
;  DE	= I2C Device Address
;  HL	= Data Pointer
;
; Outputs:
;  A	= 0 -> Success, otherwise errored
;
i2c_write:
	PUSH	HL
	PUSH	BC
	CALL	i2c_start
	JR	NZ, i2c_write_strt_failed
	LD	A, E					; I2C Device Address

	SLA	A
	AND	A, I2CMODE_TRANSMIT
	CALL	i2c_transmit_byte
	JR	NZ, i2c_write_strt_failed

	POP	BC
	POP	HL

	LD	B, C
i2c_write_loop:
	LD	A, (HL)
	PUSH	BC
	PUSH	HL
	CALL	i2c_transmit_byte
	POP	HL
	POP	BC
	JR	NZ, i2c_write_failed

	INC	HL
	DJNZ	i2c_write_loop

	XOR	A					; SUCCESS

i2c_write_failed:
	RET.L

i2c_write_strt_failed:
	POP	BC
	POP	HL
	RET.L

;
; Function B = 02 -- I2C Read (I2C_READ)
;
; Read data from the I2C bus.
;
; This function reads data from the I2C bus.  The I2C bus must be initialized before calling this function.
;
; Inputs:
;  C	= Data Length
;  DE	= I2C Device Address
;  HL	= Data Pointer
;
; Outputs:
;  A	= 0 -> Success, otherwise errored
;
i2c_read:
	LD	A, %FF					; NOT IMPLEMENTED YET
	; XOR	A					; SUCCESS
	RET.L
;
; Function B = 03 -- I2C Probe (I2C_PROBE)
;
; Test if an I2C device is present on the bus.
;;
; Inputs:
;  DE	= I2C Device Address
;
; Outputs:
;  A	= 0 -> Success, otherwise errored
;
i2c_probe:
	CALL	i2c_start
	JR	NZ, i2c_probe_failed
	LD	A, E					; I2C Device Address

	SLA	A
	AND	A, I2CMODE_TRANSMIT

	CALL	i2c_transmit_byte
	JR	NZ, i2c_probe_failed

	CALL	i2c_stop
i2c_probe_failed:
	RET.L

; -------------------------------
;

i2c_reinit:
	XOR	A
	OUT0	(I2C_SAR), A
	OUT0	(I2C_XSAR), A
	OUT0	(I2C_DR), A
	OUT0	(I2C_CTL), A
	OUT0	(I2C_CCR), A

	LD	A, I2C_SRESET
	OUT0	(I2C_SRR), A

;   CCR Register: field --0(1):M(4):N(3)
	PUSH	BC
	CALL	_i2c_calc_ccr
	POP	BC

	OUT0	(I2C_CCR),A

	LD	A, I2C_ENABLED | I2C_AAK		; bit 6 and 2
	OUT0	(I2C_CTL),A

	XOR	A					; SUCCESS
	RET
;
; Function: i2c_start
; Description: Send the I2C Start condition
;
; Inputs:
;  None
;
; Outputs:
;  A = 0 -> Success, otherwise errored code I2CERR_XXX
;  B = the retrieve value from eZ80's I2C_SR register
;  Flag: Z = Success, NZ = Error
;
; Registers used:
;  HL
;
i2c_start:
	IN0	A,(I2C_CTL)				; Enter master transmit mode:
	OR	A, I2CMASK_SET_STA			; send a start condition to begin
	AND	A, (I2CMASK_CLR_IFLG & ~I2CMASK_SET_STP) ; the transmission
	OUT0	(I2C_CTL),A

	LD	HL, COUNT_POLL_IFLG

i2cstrt_1:
	DEC	HL
	CALL	__icmpzero
	JR	Z, i2cstrt_2

	IN0	A, (I2C_CTL)				; loop while the IFLG bit is not set
	AND	A, I2CMASK_GET_IFLG
	JR	Z, i2cstrt_1

	; Wait for the IFLG bit to be cleared or a timeout
i2cstrt_2:
	IN0	A,(I2C_CTL)				; Clear/Disable the I2C interrupts
	AND	A, ~I2CMASK_SET_INT
	OUT0	(I2C_CTL),A

	IN0	A, (I2C_CTL)				; if IFLG bit is not set
	AND	A, I2CMASK_GET_IFLG			; then we have timed out
	JR	Z, i2cstrt_timeout

	IN0	A, (I2C_SR)				; read the status register
	LD	B, A					; return SR in B

	CP	I2CSTAT_STARTTXD
	JR	Z, i2cstrt_success

	CP	I2CSTAT_RPTDSTARTTXD
	JR	Z, i2cstrt_success

	CP	I2CSTAT_BUSERROR
	JR	NZ, i2cstrt_startfail

	CALL	i2c_stop

	LD	A, I2CERR_BUSERROR
	OR	A
	RET

i2cstrt_success:
	XOR	A
	RET

i2cstrt_startfail:
	LD	A, I2CERR_STARTFAIL
	OR	A
	RET

i2cstrt_timeout:
	CALL	i2c_stop
	LD	A, I2CERR_TIMEOUT
	OR	A
	RET
;
; Function: i2c_stop
; Description: Send the I2C Stop condition
;
; Inputs:
;  None
;
; Outputs:
;  A = 0 -> Success
;
i2c_stop:
	IN0	A,(I2C_CTL)				; Enter master transmit mode:
	OR	A, I2CMASK_SET_STP			; send a stop condition
	AND	A, (I2CMASK_CLR_IFLG & ~I2CMASK_SET_STA) ; the transmission
	OUT0	(I2C_CTL),A
	XOR	A
	RET
;
; Function: i2c_transmit_byte
; Description: Transmit a byte on the I2C bus
;
; Inputs:
;  A = Byte to transmit
;  Flag: Z = Success, NZ = Error
;
; Outputs:
;  A = 0 -> Success, otherwise errored code I2CERR_XXX
;  B = the retrieve value from eZ80's I2C_SR register
;  Flag: Z = Success, NZ = Error
;
; Registers used:
;  HL
;
i2c_transmit_byte:
	OUT0	(I2C_DR), A				; load byte to be transmitted

	IN0	A,(I2C_CTL)				; clear iflag bit to transmit the data byte
	AND	A, I2CMASK_CLR_IFLG
	OUT0	(I2C_CTL), A
	LD	HL, COUNT_POLL_IFLG

i2ctrnb_1:
	DEC	HL
	CALL	__icmpzero
	JR	Z, i2ctrnb_2

	IN0	A, (I2C_CTL)				; loop while the IFLG bit is not set
	AND	A, I2CMASK_GET_IFLG
	JR	Z, i2ctrnb_1

	; Wait for the IFLG bit to be cleared or a timeout
i2ctrnb_2:
	IN0	A, (I2C_CTL)				; if IFLG bit is not set
	AND	A, I2CMASK_GET_IFLG			; then we have timed out
	JR	Z, i2ctrnb_exit_with_stop

	IN0	A, (I2C_SR)				; read the status register
	LD	B, A

	CP	I2CSTAT_SLAWTXD_ACKNRXD			; %20 Address and Write bit transmitted, ACK not received
	JR	NZ, i2ctrnb_3				; ?? should we not resend?
							; send STOP, and abandon transmission
	LD	A, I2CERR_SLAWTXD_ACKNRXD
	JR	i2ctrnb_exit_with_stop

i2ctrnb_3:
	CP	I2CSTAT_DATABYTETXDMMODE_ACKRXD		; %28 Data byte transmitted in CONTROLLER mode, ACK received
	JR	Z, i2ctrnb_success
	CP	I2CSTAT_SLAWTXD_ACKRXD			; %18 Address and Write bit transmitted, ACK received
	JR	Z, i2ctrnb_success
	CP	I2CSTAT_SLARTXD_ACKRXD			; %40 Address and Read bit transmitted, ACK received
	JR	Z, i2ctrnb_success
	CP	I2CSTAT_SLDATATXD_ACKRXD		; %B8 Data byte transmitted in TARGET mode, ACK received
	JR	NZ, i2ctrnb_exit_with_stop

i2ctrnb_success:
	XOR	A					; I2CERR_SUCCESS
	RET

i2ctrnb_4:
	CP	I2CSTAT_DATABYTETXDMMODE_ACKNRXD	; %30 Data byte transmitted in CONTROLLER mode, ACK NOT received
	JR	NZ, i2ctrnb_5
	LD	A, I2CERR_DATABYTETXDMMODE_ACKNRXD
	JR	i2ctrnb_exit_with_stop

i2ctrnb_5:
	CP	I2CSTAT_ARBLOST				; %38 Arbitration lost in address or data byte
	JR	NZ, i2ctrnb_6
	LD	A, I2CERR_ARBLOST
	JR	i2ctrnb_exit_with_stop			; SPEC: CLR IFLG or CLR IFLG & SET STA

i2ctrnb_6:
	CP	I2CSTAT_ARBLOST_SLAWRXD_ACKTXD		; %68 Arbitration lost in address as CONTROLLER, target address and Write bit received, ACK transmitted
	JR	NZ, i2ctrnb_7
	LD	A, I2CERR_ARBLOST_SLAWRXD_ACKTXD	; SPEC: CLR IFLG & SET AAK
	JR	i2ctrnb_exit

i2ctrnb_7:
	CP	I2CSTAT_ARBLOST_GLCLADRXD_ACKTXD	; %78 Arbitration lost in address as CONTROLLER, General Call address received, ACK transmitted
	JR	NZ, i2ctrnb_8
	LD	A, I2CERR_ARBLOST_GLCLADRXD_ACKTXD	; SPEC: CLR IFLG & SET AAK
	JR	i2ctrnb_exit

i2ctrnb_8:
	CP	I2CSTAT_ARBLOST_SLARRXD_ACKTXD		; %B0 Arbitration lost in address as CONTROLLER, target address and Read bit received, ACK transmitted
	JR	NZ, i2ctrnb_9
	LD	A, I2CERR_ARBLOST_SLARRXD_ACKTXD
	JR	i2ctrnb_exit

i2ctrnb_9:
	CP	I2CSTAT_SLARTXD_ACKNRXD			; %48 Address and Read bit transmitted, ACK NOT received
	JR	NZ, i2ctrnb_10
	LD	A, I2CERR_SLARTXD_ACKNRXD
	JR	i2ctrnb_exit

i2ctrnb_10:
	CP	I2CSTAT_SLDATATXD_ACKNRXD		; %C0 Data byte transmitted in TARGET mode, ACK not received
	JR	NZ, i2ctrnb_11

	CALL	i2c_reinit
	LD	A, I2CERR_IDLE
	JR	i2ctrnb_exit

i2ctrnb_11:
	CALL	i2c_reinit
	LD	A, I2CERR_FAILURE
	JR	i2ctrnb_exit

i2ctrnb_timeout:
	CALL	i2c_stop
	LD	A, I2CERR_TIMEOUT

i2ctrnb_exit:
	OR	A
	RET

i2ctrnb_exit_with_stop:
	PUSH	AF
	CALL	i2c_stop
	POP	AF
	OR	A
	RET
