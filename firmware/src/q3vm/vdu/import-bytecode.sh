#!/usr/bin/env bash

set -e

PATH=${PATH}:../../q3vm/tools/

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

IMAGE_NAME=vdu_vm_bytecode
IMAGE_NAME_UPCASE=VDU_VM_BYTECODE
OUTPUT_FILE="../../rst-10-drivers/crt/${IMAGE_NAME}"

cat <<EOT > "${OUTPUT_FILE}.h"
#ifndef __${IMAGE_NAME_UPCASE}
#define __${IMAGE_NAME_UPCASE}

#include <stdint.h>

#define ${IMAGE_NAME_UPCASE}_SIZE ${bytecodeSize}
#define ${IMAGE_NAME_UPCASE}_RAM_SIZE ${ramRequired}

extern const uint8_t ${IMAGE_NAME}[${IMAGE_NAME_UPCASE}_SIZE];

extern uint8_t ${IMAGE_NAME}_ram[${IMAGE_NAME_UPCASE}_RAM_SIZE];

#endif

EOT

cat <<EOT > "${OUTPUT_FILE}.c"
#include <stdint.h>
#include "${IMAGE_NAME}.h"

const uint8_t ${IMAGE_NAME}[${IMAGE_NAME_UPCASE}_SIZE] = {
EOT


xxd -c 1 ./bytecode.qvm |  cut -d' ' -f2 | while read hex; do
  printf "0x" >> ./${OUTPUT_FILE}.c
  printf $hex >> ./${OUTPUT_FILE}.c
  printf "," >> ./${OUTPUT_FILE}.c
  #Do something with $hex
done


cat <<EOT >> ${OUTPUT_FILE}.c
};

uint8_t ${IMAGE_NAME}_ram[${IMAGE_NAME_UPCASE}_RAM_SIZE];

EOT

clang-format -i "${OUTPUT_FILE}.c" "${OUTPUT_FILE}.h"
