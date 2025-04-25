#!/usr/bin/env bash

set -e

echo "Compiling usb.c -> usb.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S usb.c -c -o usb.s

