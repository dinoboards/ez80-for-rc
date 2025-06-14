#!/bin/bash

input="./bin/z80all.bin"
output="../bin/zexall.asm"

: > "$output"  # Truncate output file


printf "IFDEF ZEXALL\r\n" > "$output"
printf ".assume	adl=0\r\n" >> "$output"
printf "global	zexall\r\n" >> "$output"
printf "global	zexall_end\r\n" >> "$output"
printf "zexall:\r\n" >> "$output"

# Use xxd to get hex bytes, then output .db lines
xxd -p -c1 "$input" | while read byte; do
    printf "\tdb %%%s\r\n" "$byte"
done >> "$output"

printf "zexall_end:\r\n" >> "$output"

printf "ENDIF\r\n" >> "$output"
