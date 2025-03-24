#!/usr/bin/env bash

set -e

echo "Compiling suphdmi.c -> suphdmi.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S suphdmi.c -c -o suphdmi.s

