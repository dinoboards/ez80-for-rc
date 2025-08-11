#!/usr/bin/env bash

set -e

PATH=${PATH}:../tools/

lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -o main.vmasm main.c

q3asm -s 512 -m -v -f bytecode

bytecodeSize=$(stat -c %s "bytecode.qvm")

dataLength=$(xxd -s 20 -g 4 -e -l 4 -R never ./bytecode.qvm | cut -d' ' -f2)
bssLength=$(xxd -s 28 -g 4 -e -l 4 -R never ./bytecode.qvm | cut -d' ' -f2)

ramRequired=$((0x$dataLength+0x$bssLength))
echo data: ${dataLength}
echo bss: ${bssLength}
echo bytecodeSize: ${bytecodeSize}
echo ramRequired: ${ramRequired}

cat <<EOT > spike.h
#ifndef __SPIKE
#define __SPIKE

#include <stdint.h>

#define VM_VDU_IMAGE_SIZE ${bytecodeSize}
#define VM_VDU_RAM_SIZE ${ramRequired}

extern const uint8_t vm_vdu_image[VM_VDU_IMAGE_SIZE];

extern uint8_t vm_vdu_ram[VM_VDU_RAM_SIZE];

#endif

EOT

cat <<EOT > spike.c
#include <stdint.h>
#include "spike.h"

const uint8_t vm_vdu_image[VM_VDU_IMAGE_SIZE] = {
EOT


xxd -c 1 ./bytecode.qvm | while read offset hex char; do
  printf "0x" >> ./spike.c
  printf $hex >> ./spike.c
  printf "," >> ./spike.c
  #Do something with $hex
done


cat <<EOT >> spike.c
};

uint8_t vm_vdu_ram[VM_VDU_RAM_SIZE];

EOT

clang-format -i spike.c spike.h
