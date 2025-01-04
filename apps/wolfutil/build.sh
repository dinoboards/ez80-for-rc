#!/usr/bin/env bash

set -e

echo "Compiling test_rendering_tiles.c -> test_rendering_tiles.s"
ez80-clang ${INCLUDE_PATHS} -nostdinc -ffunction-sections -fdata-sections -Oz -Wall -Wextra -Wunreachable-code -Werror -mllvm -z80-print-zero-offset -S test_rendering_tiles.c -c -o test_rendering_tiles.s

