#!/usr/bin/env bash

set -e

echo "Compiling report-memory-timing.c -> report-memory-timing.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S report-memory-timing.c -c -o report-memory-timing.s

