#!/bin/bash

set -e

ZXCC=../tools/zxcc/zxcc
MAC=../tools/MAC

  FILENAME=$1

  trap "rm -f tmp.*" EXIT
  trap "rm -f $FILENAME.hex" EXIT

  printf "\r\n; FILE: $FILENAME\r\n\r\n" > "$FILENAME.s"

  printf "\tsection\tTEXT\r\n\t.assume\tadl=1\r\n\r\n" >> "$FILENAME.s"

  printf "\tPUBLIC\tIMAGE_${FILENAME^^}\r\n"  >> "$FILENAME.s"
  printf "\tPUBLIC\tIMAGE_${FILENAME^^}_END\r\n"  >> "$FILENAME.s"
  printf "\tPUBLIC\tIMAGE_${FILENAME^^}_SIZE\r\n\r\n"  >> "$FILENAME.s"

  printf "IMAGE_${FILENAME^^}:\r\n"  >> "$FILENAME.s"

  xxd -p -c 16 "./${FILENAME}.bin" | while read -r line; do
    formatted_line=$(echo "$line" | sed 's/../0&H, /g;s/, $/ /')
    printf "\tDB\t${formatted_line%} \r\n" >> "$FILENAME.s"
  done


  printf "\r\nIMAGE_${FILENAME^^}_END:\r\n"  >> "$FILENAME.s"
  printf "\r\nIMAGE_${FILENAME^^}_SIZE:\tEQU\tIMAGE_${FILENAME^^}_END-IMAGE_${FILENAME^^}\r\n"  >> "$FILENAME.s"



