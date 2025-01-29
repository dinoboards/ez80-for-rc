#!/usr/bin/env bash

set -x

ARG1=${1:-ez80-cpu-v1.7}
NAME="${ARG1%.pld}"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

mkdir -p ./bin

wine cupl.exe -a -l -m4 -e -x -f -ju ${WINCUPLPATH}\cupl.dl  $NAME 2>>./bin/wincuplerror.log

# Define ANSI color codes
RED='\033[0;31m'
NC='\033[0m' # No Color

if [ -f "${NAME}.jed" ]; then
# Removed the location entry - the TL866II Plus programmer seems to have loading file issue
  sed -i '/Location/d' ./${NAME}.jed
  mv ${NAME}.jed ./bin
else
  # Print text in red
  echo -e "${RED}"
  wine find1502 ${NAME}.tt2
  echo -e "${NC}"
  exit 1
fi

mv ${NAME}.doc bin/${NAME}.txt
mv ${NAME}.mx bin/${NAME}.mx

[ -f "${NAME}.fit" ] && mv ${NAME}.fit bin/${NAME}.fit
[ -f "${NAME}.io" ] && mv ${NAME}.io bin/${NAME}.io
[ -f "${NAME}.abs" ] && mv ${NAME}.abs bin/${NAME}.abs
[ -f "${NAME}.pin" ] && mv ${NAME}.pin bin/${NAME}.pin
[ -f "${NAME}.pla" ] && mv ${NAME}.pla bin/${NAME}.pla
[ -f "${NAME}.sim" ] && mv ${NAME}.sim bin/${NAME}.sim
[ -f "${NAME}.tt2" ] && mv ${NAME}.tt2 bin/${NAME}.tt2
[ -f "${NAME}.tt3" ] && mv ${NAME}.tt3 bin/${NAME}.tt3
[ -f "${NAME}.lst" ] && mv ${NAME}.lst bin/${NAME}.lst
