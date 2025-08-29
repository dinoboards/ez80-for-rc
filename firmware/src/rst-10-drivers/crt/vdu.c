#include "../../q3vm/host/target-support.h"
#include "../../q3vm/host/vm.h"

extern vm_t vm;

void vdu(uint8_t code) { VM_Call(&vm, 0, code); }
