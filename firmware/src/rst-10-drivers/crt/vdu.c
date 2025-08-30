#include "../../q3vm/host/target-support.h"
#include "../../q3vm/host/vm.h"

extern vm_t vm;

typedef struct point_s {
  int16_t x;
  int16_t y;
} point_t;

point_t point_new(int16_t x, int16_t y) {
  point_t t;
  t.x = x;
  t.y = y;

  return t;
}

void vdu(uint8_t code) {

  point_t x = point_new(1, 2);

  point_t y = {3, 4};

  VM_Call(&vm, 0, code);
}
