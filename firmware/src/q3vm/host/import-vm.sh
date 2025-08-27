#!/usr/bin/env bash

set -e

cp -u /opt/ez80-q3vm/includes/vm.c ./
cp -u /opt/ez80-q3vm/includes/vm.h ./
cp -u /opt/ez80-q3vm/includes/opcodes.h ./
cp -u /opt/ez80-q3vm/includes/opcodes.c.h ./

# cp ~/dinoboards/q3vm/verification/tests-byte-code.c ../../../../apps/q3vm/
# cp ~/dinoboards/q3vm/verification/tests-byte-code.h ../../../../apps/q3vm/
