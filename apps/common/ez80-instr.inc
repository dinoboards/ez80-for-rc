; RST.L $08
#define EZ80_IO		DB 	$49, $CF
; RST.L $10
#define EZ80_FN 	DB	$49, $D7
; RST.L $18
#define EZ80_BNKSEL 	DB	$49, $DF

#define EZ80_UTIL_VER_EXCH		XOR A \ \
					LD B, 0 \ \
					EZ80_FN

#define EZ80_UTIL_GET_BUSTM 		XOR A \ \
					LD B, 1 \ \
					EZ80_FN

#define EZ80_UTIL_SET_BUSTM 		XOR A \ \
					LD B, 3 \ \
					EZ80_FN

#define EZ80_UTIL_SET_BUSFQ 		XOR A \ \
					LD B, 4 \ \
					EZ80_FN

#define EZ80_UTIL_GET_CPU_FQ 		XOR A \ \
					LD B, 5 \ \
					EZ80_FN

#define EZ80_UTIL_DEBUG 		XOR A \ \
					LD B, 7 \ \
					EZ80_FN

#define EZ80_RTC_INIT 			LD A, 1 \ \
					LD B, 0 \ \
					EZ80_FN

#define EZ80_RTC_GET_TIME 		LD A, 1	\ \
					LD B, 1 \ \
					EZ80_FN

#define EZ80_RTC_SET_TIME 		LD A, 1	\ \
					LD B, 2 \ \
					EZ80_FN

#define EZ80_TMR_GET_TICKS		LD A, 2 \ \
					LD B,  0 \ \
					EZ80_FN

#define EZ80_TMR_GET_SECONDS		LD A, 2 \ \
					LD B,  1 \ \
					EZ80_FN

#define EZ80_TMR_SET_TICKS		LD A, 2 \ \
					LD B,  2 \ \
					EZ80_FN

#define EZ80_TMR_SET_SECONDS		LD A, 2 \ \
					LD B,  3 \ \
					EZ80_FN

#define EZ80_TMR_GET_FREQTICK		LD A, 2 \ \
					LD B,  4 \ \
					EZ80_FN

#define EZ80_TMR_SET_FREQTICK		LD A, 2 \ \
					LD B,  5 \ \
					EZ80_FN

#define EZ80_TMR_THROTTLE_START		LD A, 2 \ \
					LD B,  6 \ \
					EZ80_FN

#define EZ80_TMR_THROTTLE_WAIT		LD A, 2 \ \
					LD B,  7 \ \
					EZ80_FN

#define EZ80_TMR_INT_DISABLE		LD A, 2 \ \
					LD B,  8 \ \
					EZ80_FN

#define EZ80_TMR_INT_ENABLE		LD A, 2 \ \
					LD B,  9 \ \
					EZ80_FN

#define EZ80_TMR_IS_TICK_ISR		LD A, 2 \ \
					LD B, 10 \ \
					EZ80_FN

#define EZ80_TMR_DELAY			LD A, 2 \ \
					LD B, 11 \ \
					EZ80_FN

#define EZ80_THROTTLE_START(p,store) \
					PUSH AF \ \
					PUSH BC \ \
					PUSH HL \ \
					LD A, 2 \ \
					LD BC, (6 * 256) + p \ \
					EZ80_FN \ \
					LD (store), HL \ \
					POP HL \ \
					POP BC \ \
					POP AF

#define EZ80_THROTTLE_WAIT(p,store) \
					PUSH AF \ \
					PUSH BC \ \
					PUSH HL \ \
					LD A, 2 \ \
					LD BC, (7 * 256) + p \ \
					LD HL, (store) \ \
					EZ80_FN \ \
					LD (store), HL \ \
					POP HL \ \
					POP BC \ \
					POP AF

#define EZ80_UART_IN			LD A, 3 \ \
					LD B, 0 \ \
					EZ80_FN
#define EZ80_UART_OUT			LD A, 3 \ \
					LD B, 1 \ \
					EZ80_FN

#define EZ80_UART_IN_STAT		LD A, 3 \ \
					LD B, 2 \ \
					EZ80_FN

#define EZ80_UART_OUT_STAT		LD A, 3 \ \
					LD B, 3 \ \
					EZ80_FN

#define EZ80_UART_CONFIG		LD A, 3 \ \
					LD B, 4 \ \
					EZ80_FN

#define EZ80_UART_QUERY			LD A, 3 \ \
					LD B, 5 \ \
					EZ80_FN

#define EZ80_UART_RESET			LD A, 3 \ \
					LD B, 6 \ \
					EZ80_FN

#define RET_L				DB	$49 \ \
					RET

#define IN0_A(p)	DB	$ED,$38,p
#define IN0_B(p)	DB	$ED,$00,p
#define IN0_C(p)	DB	$ED,$08,p
#define IN0_D(p)	DB	$ED,$10,p
#define IN0_E(p)	DB	$ED,$18,p
#define IN0_H(p)	DB	$ED,$20,p
#define IN0_L(p)	DB	$ED,$28,p

#define OUT0_A(p)	DB	$ED,$39,p
#define OUT0_B(p)	DB	$ED,$01,p
#define OUT0_C(p)	DB	$ED,$09,p
#define OUT0_D(p)	DB	$ED,$11,p
#define OUT0_E(p)	DB	$ED,$19,p
#define OUT0_H(p)	DB	$ED,$21,p
#define OUT0_L(p)	DB	$ED,$29,p

#define LDHLMM_LIL(Mmn)	\
					DB	$5B \ \
					LD	HL, Mmn \ \
					DB	(Mmn >> 16) & $FF

#define LDBCMM_LIL(Mmn)			DB	$5B \ \
					LD	BC, Mmn \ \
					DB	(Mmn >> 16) & $FF

#define SBCHLBC_LIL \ \
					DB	$49 \ \
					SBC	HL, BC

#define LDIR_LIL 			DB	$5B \ \
					LDIR

IO_SEGMENT	EQU		$FF		; THE UPPER 8-BIT ADDRESS FOR I/O

#define OUT_NN_A(addr) \
					PUSH	BC \ \
					LD	BC, IO_SEGMENT << 8 | addr \ \
					OUT	(C), A \ \
					POP	BC

#define IN_A_NN(addr) \
					LD A, IO_SEGMENT \ \
					IN A, (addr)


	EXTERN _EZ80_CPY_EHL_TO_UHL
#define EZ80_CPY_EHL_TO_UHL 	CALL _EZ80_CPY_EHL_TO_UHL

	EXTERN _EZ80_CPY_UHL_TO_EHL
#define EZ80_CPY_UHL_TO_EHL	CALL _EZ80_CPY_UHL_TO_EHL

	EXTERN	_EZ80_CPY_MBHL_TO_UHL
#define EZ80_CPY_MBHL_TO_UHL	CALL _EZ80_CPY_MBHL_TO_UHL

	EXTERN	_EZ80_CPY_MBDE_TO_UDE
#define EZ80_CPY_MBDE_TO_UDE	CALL _EZ80_CPY_MBDE_TO_UDE

	EXTERN	_EZ80_CPY_AUBC_TO_DEHL
#define EZ80_CPY_AUBC_TO_DEHL	CALL _EZ80_CPY_AUBC_TO_DEHL

