#ifndef _HBIOS_TIMER_H_
#define _HBIOS_TIMER_H_

#include "hbios_sysget_tick.h"

#define native_timer_start() sysget_tick()
#define native_timer_stop()  sysget_tick()
#define native_timer_rate()  sysget_tick_rate()

#endif
