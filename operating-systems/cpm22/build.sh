#!/bin/bash

set -e

ZXCC=../tools/zxcc/zxcc
MAC=../tools/MAC

FILENAME=$1

trap "rm -f tmp.* $FILENAME.hex" EXIT

echo "Assembling ${FILENAME^^}.ASM"
${ZXCC} ${MAC} -$FILENAME.asm -$PO > ./tmp.txt
pr -to 4 ./tmp.txt
tr -d '\032' < $FILENAME.hex > ./tmp.hex

objcopy -I ihex -O binary ./tmp.hex ./tmp.bin
dd status=none if=./tmp.bin of=${FILENAME}.bin bs=128 conv=sync

