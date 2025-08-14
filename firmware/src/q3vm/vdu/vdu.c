#include "bg_lib.h"
#include <host-functions.h>
#include <stdint.h>

void vdu(uint8_t code) {

  uint8_t  ua;
  int8_t   a;
  uint16_t ub;
  int16_t  b;
  uint32_t ud;
  int32_t  d;

  float f;

  ua = 123;
  a  = -123;
  ub = 45000;
  b  = -32000;
  ud = 0x12345678;
  d  = 0x11223344;

  f = 1.345;

  printf("Char %d %c\r\n\n", code, code);
  printf("ua: %d, a: %d, ub: %d, b: %d, ud: %d, d:%d, f: %f\r\n", ua, a, ub, b, ud, d, f);
}
