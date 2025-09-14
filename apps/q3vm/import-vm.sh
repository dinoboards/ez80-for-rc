#!/usr/bin/env bash

set -e

cp -u /opt/ez80-q3vm/includes/vm.c ./
cp -u /opt/ez80-q3vm/includes/vm.h ./
cp -u /opt/ez80-q3vm/includes/opcodes.h ./
cp -u /opt/ez80-q3vm/includes/opcodes.c.h ./

cp /opt/ez80-q3vm/includes/verifications-byte-code.c ./
cp /opt/ez80-q3vm/includes/verifications-byte-code.h ./
