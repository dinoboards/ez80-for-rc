#!/usr/bin/env bash

set +x

ARG1=${1:-ez80-cpu-v1.7}
NAME="${ARG1%.pld}"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

WINCUPLPATH=/Wincupl/Shared/

mkdir -p ./bin

wine ${WINCUPLPATH}cupl.exe -e -x -f - -ju ${WINCUPLPATH}\cupl.dl $NAME 2>>./bin/wincuplerror.log

# Removed the location entry - the TL866II Plus programmer seems to have loading file issue
sed -i '/Location/d' ./${NAME}.jed

mv ${NAME}.jed ./bin
mv ${NAME}.doc bin/${NAME}.txt
mv ${NAME}.mx bin/${NAME}.mx
