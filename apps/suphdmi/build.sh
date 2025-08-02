#!/usr/bin/env bash

set -e

echo "Compiling test_logic_remap.c -> test_logic_remap.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S test_logic_remap.c -c -o test_logic_remap.s  -DVDP_SUPER_HDMI

echo "Compiling suphdmi.c -> suphdmi.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -O3 -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S suphdmi.c -c -o suphdmi.s -DVDP_SUPER_HDMI

