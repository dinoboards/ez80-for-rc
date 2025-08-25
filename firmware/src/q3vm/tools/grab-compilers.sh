#!/usr/bin/env bash

set -e

cp -u ~/dinoboards/q3vm/bin/linux/lcc ./
cp -u ~/dinoboards/q3vm/bin/linux/q3asm ./
cp -u ~/dinoboards/q3vm/bin/linux/q3cpp ./
cp -u ~/dinoboards/q3vm/bin/linux/q3rcc ./

cp -u ~/dinoboards/q3vm/src/vm/vm.c ../host/
cp -u ~/dinoboards/q3vm/src/vm/vm.h ../host/
cp -u ~/dinoboards/q3vm/src/vm/opcodes.h ../host/
cp -u ~/dinoboards/q3vm/src/vm/opcodes.c.h ../host/

cp ~/dinoboards/q3vm/verification/tests-byte-code.c ../../../../apps/q3vm/
cp ~/dinoboards/q3vm/verification/tests-byte-code.h ../../../../apps/q3vm/
