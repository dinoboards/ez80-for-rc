#!/usr/bin/env bash

set -e

echo "Compiling find-extended-memory.c -> find-extended-memory.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S find-extended-memory.c -c -o find-extended-memory.s

