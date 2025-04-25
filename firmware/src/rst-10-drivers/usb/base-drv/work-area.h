#ifndef __WORK_AREA
#define __WORK_AREA

#include "ch376.h"
#include "protocol.h"
#include "stdlib.h"
#include "usb_state.h"

#define PRES_CF    1   /* BIT MASK FOR COMPACTFLASH PRESENT */
#define PRES_MS    2   /* BIT MASK FOR MSX MUSIC NOR FLASH PRESENT */
#define PRES_USB1  4   /* BIT MASK FOR USB1 STORAGE PRESENT AT BOOT UP */
#define PRES_USB2  8   /* BIT MASK FOR USB2 STORAGE PRESENT AT BOOT UP */
#define PRES_USB3  16  /* BIT MASK FOR USB3 STORAGE PRESENT AT BOOT UP */
#define PRES_USB4  32  /* BIT MASK FOR USB4 STORAGE PRESENT AT BOOT UP */
#define PRES_CH376 128 /* BIT MASK FOR CH376 PRESENT AT BOOT UP */

#define BIT_PRES_CF    0 /* BIT POSTION FOR COMPACTFLASH PRESENT */
#define BIT_PRES_MS    1 /* BIT POSTION FOR MSX MUSIC NOR FLASH PRESENT */
#define BIT_PRES_USB1  2 /* BIT POSTION FOR USB1 STORAGE PRESENT */
#define BIT_PRES_USB2  3 /* BIT POSTION FOR USB2 STORAGE PRESENT */
#define BIT_PRES_USB3  4 /* BIT POSTION FOR USB3 STORAGE PRESENT */
#define BIT_PRES_USB4  5 /* BIT POSTION FOR USB4 STORAGE PRESENT */
#define BIT_PRES_CH376 7 /* BIT POSTION FOR CH376 PRESENT */

typedef uint8_t device_map_t;
enum device_map_e {
  DEV_MAP_NONE = 0,
  DEV_MAP_ROM  = 1,
  DEV_MAP_CF   = 2,
  DEV_MAP_MS   = 3,
  DEV_MAP_USB1 = 4,
  DEV_MAP_USB2 = 5,
  DEV_MAP_USB3 = 6,
  DEV_MAP_USB4 = 7
};

extern uint8_t get_number_of_usb_drives(void);

extern usb_state_t x;

#define get_usb_work_area() (&x)

#endif
