#!/bin/bash

set -e

output=debug/cpm22.img
Label="CP/M 2.2"
SysImage="../cpm22/debug/cpm22img.sys"

fmt=wbw_hd1k
dtype=hd
type=hd1k_

HD1KSIZE=8192

mkdir -p debug
LC_ALL=en_US.US-ASCII tr '\000' '\345' </dev/zero | dd status=none of="${output}" bs=1024 count=${HD1KSIZE}

mkfs.cpm -f $fmt -b ${SysImage} ${output}

Label=`echo "${Label}" | awk '{ print substr($0, 1, 16) }'`
# echo "Label: \"${Label}\""
echo "\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$" | tr -d '\n' | dd conv=notrunc status=none bs=1 seek=1511 of=${output}
echo "${Label}" | tr -d '\n' | dd conv=notrunc status=none bs=1 seek=1511 of=${output} 2>/dev/null

for file in ./cpm22/u0/*; do
  cpmcp -f "${fmt}" "${output}" "$file" "0:"
done
