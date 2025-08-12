#!/usr/bin/env bash

set -e

PATH=${PATH}:../tools/

lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -o main.vmasm main.c

q3asm -s 512 -m -v -f bytecode

bytecodeSize=$(stat -c %s "bytecode.qvm")

# typedef struct {
#   uint32_t vmMagic;          /**< 00: Bytecode image shall start with VM_MAGIC */
#   uint24_t instructionCount; /**< 04: Number of instructions in .qvm */
#   uint24_t codeOffset;       /**< 07: Byte offset in .qvm file of .code segment */
#   uint24_t codeLength;       /**< 0A: Bytes in code segment */
#   uint24_t dataOffset;       /**< 0D: Byte offset in .qvm file of .data segment */
#   uint24_t dataLength;       /**< 10: Bytes in .data segment */
#   uint24_t litLength;        /**< 13: Bytes in strings segment (after .data segment) */
#   uint24_t bssLength;        /**< 16: How many bytes should be used for .bss segment */
# } vmHeader_t;

dataLengthL=$(xxd -s 19 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
dataLengthM=$(xxd -s 20 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
dataLengthH=$(xxd -s 21 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
dataLength="${dataLengthH}${dataLengthM}${dataLengthL}"

bssLengthL=$(xxd -s 22 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
bssLengthM=$(xxd -s 23 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
bssLengthH=$(xxd -s 24 -g 1 -e -l 1 -R never ./bytecode.qvm | cut -d' ' -f2)
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
