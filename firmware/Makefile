SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory

.PHONY: debug
debug: #VM_VDU src/q3vm/src/system-calls-ids.h
	@wine make-debug.bat

.PHONY: release
release: VM_VDU
	@wine make-release.bat

src/q3vm/src/system-calls-ids.asm src/q3vm/src/system-calls-ids.h: src/q3vm/src/system-calls.txt
	cd src/q3vm/src && ./system-calls-generator.sh

.PHONY: VM_VDU
VM_VDU: src/q3vm/src/system-calls-ids.asm
	@$(MAKE) -C ./src/q3vm/src
