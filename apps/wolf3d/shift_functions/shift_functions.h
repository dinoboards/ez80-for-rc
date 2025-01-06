#ifndef __SHIFT_FUNCTIONS_H__
#define __SHIFT_FUNCTIONS_H__

#include <stdint.h>

extern int16_t sr_s32_s16_16(int32_t val);
extern int32_t sr_s32_s32_8(int32_t val);
extern int32_t sr_s32_s32_6(int32_t val);
extern int32_t sr_s32_s32_4(int32_t val);
extern int32_t sr_s32_s32_11(int32_t val);

extern int32_t sl_s16_s32_16(int16_t val);
extern int32_t sl_s16_s32_10(int16_t val);

extern uint24_t sr_u24_u24_3(uint24_t val);

#endif
