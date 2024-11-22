#!/usr/bin/env bash

set -e

echo "Compiling id_mm.c -> id_mm.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S id_mm.c -c -o id_mm.s

