#!/usr/bin/env bash

set -e

echo "code" > ./system-calls-ids.asm
echo "" >> ./system-calls-ids.asm

echo "#ifndef __SYSTEMCALLSIDS__" > ./system-calls-ids.h
echo "#define __SYSTEMCALLSIDS__" >> ./system-calls-ids.h
echo "" >> ./system-calls-ids.h


count=-1
while IFS= read -r name; do
  printf "equ\t%-35s %5d\n" ${name} ${count} >> ./system-calls-ids.asm
  printf "#define SC_${name^^} ${count}\n" >> ./system-calls-ids.h
  count=$((count - 1))


done < system-calls.txt

echo "" >> ./system-calls-ids.asm

printf "\n#endif\n" >> ./system-calls-ids.h

clang-format -i ./system-calls-ids.h
