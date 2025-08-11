#!/usr/bin/env bash

set -e

cp ~/dinoboards/q3vm/bin/linux/lcc ./
cp ~/dinoboards/q3vm/bin/linux/q3asm ./
cp ~/dinoboards/q3vm/bin/linux/q3cpp ./
cp ~/dinoboards/q3vm/bin/linux/q3rcc ./

cp ~/dinoboards/q3vm/src/vm/vm.c ../host/
cp ~/dinoboards/q3vm/src/vm/vm.h ../host/

