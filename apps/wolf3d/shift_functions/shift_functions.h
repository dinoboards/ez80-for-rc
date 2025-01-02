#ifndef __SHIFT_FUNCTIONS_H__
#define __SHIFT_FUNCTIONS_H__

#include <stdint.h>

extern int16_t sr_s32_s16_16(int32_t val);
extern int32_t sr_s32_s32_8(int32_t val);
extern int32_t sr_s32_s32_4(int32_t val);
extern int32_t sr_s32_s32_11(int32_t val);

// extern uint16_t rr_u24_8(uint24_t val);
// extern uint24_t rr_u24_6(uint24_t val);
// extern uint24_t rr_u24_1(uint24_t val);

// extern int16_t rr_s24_8(int24_t val);
// extern int24_t rr_s24_6(int24_t val);
// extern int24_t rr_s24_1(int24_t val);

// extern uint32_t rl_u16_16(uint16_t val);

// extern int32_t rl_s16_16(int16_t val);

#endif
