#include <stdint.h>
#include <stdio.h>

extern uint16_t reg_aaf;
extern uint16_t reg_abc;
extern uint16_t reg_ade;
extern uint16_t reg_ahl;
extern uint16_t reg_ix;
extern uint16_t reg_iy;
extern uint16_t reg_af;
extern uint16_t reg_bc;
extern uint16_t reg_de;
extern uint16_t reg_hl;
extern uint16_t reg_sp;
extern uint8_t *reg_pc;

#define bc reg_pc
#define pc reg_pc
#define af reg_af

#define as_uint16_t(x) (*((uint16_t*)x))
