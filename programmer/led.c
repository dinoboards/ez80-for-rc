#include "led.h"
#include "ez80f92.h"
#include "zdi.h"

/*
        LD	A, %FE				; configure PC0 as output
        OUT0	(PC_DDR), A
        XOR	A
        OUT0	(PC_ALT1), A			;
        OUT0	(PC_ALT2), A

  LD  A, 0
  OUT0	(PC_DR), A  ; turn led on

  LD  A, 1
  OUT0	(PC_DR), A  ; turn led off
*/

static void configure_led_gpio() {
  zdi_debug_break();
  zdi_instr_di();
  zdi_set_mode_adl();

  zdi_load_a_nn(0xFE);
  zdi_out0_nn_a(PC_DDR);
  zdi_load_a_nn(0x00);
  zdi_out0_nn_a(PC_ALT1);
  zdi_out0_nn_a(PC_ALT2);
}

void turn_led_off(void) {
  configure_led_gpio();

  zdi_load_a_nn(0x01);
  zdi_out0_nn_a(PC_DR);
}

void turn_led_on(void) {
  configure_led_gpio();

  zdi_load_a_nn(0x00);
  zdi_out0_nn_a(PC_DR);
}
