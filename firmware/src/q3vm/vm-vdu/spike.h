#ifndef __SPIKE
#define __SPIKE

#include <stdint.h>

#define VM_VDU_IMAGE_SIZE 128
#define VM_VDU_RAM_SIZE   512

extern const uint8_t vm_vdu_image[VM_VDU_IMAGE_SIZE];

extern uint8_t vm_vdu_ram[VM_VDU_RAM_SIZE];

#endif
