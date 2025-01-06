#!/usr/bin/env bash

set -e

echo "Compiling test_fixed_point.c -> test_fixed_point.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S test_fixed_point.c -c -o test_fixed_point.s

