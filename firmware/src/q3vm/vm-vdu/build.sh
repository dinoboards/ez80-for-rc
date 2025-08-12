#!/usr/bin/env bash

set -e

PATH=${PATH}:../tools/

lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -o main.vmasm main.c

q3asm -s 512 -m -v -f bytecode

bytecodeSize=$(stat -c %s "bytecode.qvm")

# # typedef struct {
#   uint32_t vmMagic;          /**< 00: Bytecode image shall start with VM_MAGIC */
#   uint24_t instructionCount; /**< 04: Number of instructions in .qvm */
#   uint24_t codeLength;       /**< 07: Bytes in code segment */
#   uint24_t litLength;        /**< 0A: Bytes in strings segment (after .data segment) */
#   uint24_t dataLength;       /**< 0D: Bytes in .data segment */
#   uint24_t bssLength;        /**< 10: How many bytes should be used for .bss segment */
# # } vmHeader_t;

dataLengthL=$(xxd -s 13 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
dataLengthM=$(xxd -s 14 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
dataLengthH=$(xxd -s 15 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
dataLength="${dataLengthH}${dataLengthM}${dataLengthL}"

bssLengthL=$(xxd -s 16 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
bssLengthM=$(xxd -s 17 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
bssLengthH=$(xxd -s 18 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
bssLength="${bssLengthH}${bssLengthM}${bssLengthL}"

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
