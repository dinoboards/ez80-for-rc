#!/bin/bash

set -x

(cd / && find -iname sourcesanspro.sty)

version=${version:-$(date +%y-%m-%d)}
input_file=$1
output_file=$2

# Process with gpp
# gpp -I ./docs -o "${output_file}.tmp" \
#     -U "$$" "$$" "{" "}{" "}$$" "{" "}" "@@@" "" \
#     -M "$$" "$$" "{" "}{" "}$$" "{" "}" \
#     -Dversion="${version}" \
#     "${input_file}"

# Generate PDF with pandoc
pandoc "${input_file}" \
    -f markdown \
    --template "/docs/eisvogel.tex" \
    --listings \
    -t latex -s \
    -o "${output_file}" \
    --default-image-extension=pdf \
    --resource-path /docs/

# Clean up
rm "${output_file}.tmp"
