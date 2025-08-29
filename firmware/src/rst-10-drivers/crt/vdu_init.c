#include "../../q3vm/host/vm.h"

extern vm_t vm;

void vdu_init() {
  VM_Call(&vm, 255);
}
