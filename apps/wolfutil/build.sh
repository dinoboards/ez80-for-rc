#!/usr/bin/env bash

set -e

echo "Compiling vdp_cpu_to_vram.c -> vdp_cpu_to_vram.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S vdp_cpu_to_vram.c -c -o vdp_cpu_to_vram.s

