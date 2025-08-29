SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory

.PHONY: debug
debug: VM_VDU
	@wine make-debug.bat

.PHONY: release
release: VM_VDU
	@wine make-release.bat

.PHONY: VM_VDU
VM_VDU:
	@$(MAKE) -C ./src/q3vm/src
