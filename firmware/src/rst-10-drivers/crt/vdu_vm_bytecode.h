#ifndef __VDU_VM_BYTECODE
#define __VDU_VM_BYTECODE

#include <stdint.h>

#define VDU_VM_BYTECODE_SIZE     11606
#define VDU_VM_BYTECODE_RAM_SIZE 737

extern const uint8_t vdu_vm_bytecode[VDU_VM_BYTECODE_SIZE];

extern uint8_t vdu_vm_bytecode_ram[VDU_VM_BYTECODE_RAM_SIZE];

#endif
