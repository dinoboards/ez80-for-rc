SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory

VM_TOOLS := $(wildcard ./src/q3vm/tools/*)
VM_VDU_C_SOURCES := $(wildcard src/q3vm/vdu/*.c)
VM_VDU_H_SOURCES := $(wildcard src/q3vm/vdu/*.h)
VM_VDU_BYTECODE := src/rst-10-drivers/crt/vdu_vm_bytecode.c

$(shell cd src/q3vm/tools && ./grab-compilers.sh > /dev/null)

.PHONY: debug
debug: $(VM_VDU_BYTECODE)
	@wine make-debug.bat

.PHONY: release
release: $(VM_VDU_BYTECODE)
	@wine make-release.bat

$(VM_VDU_BYTECODE): $(VM_VDU_C_SOURCES) $(VM_VDU_H_SOURCES) $(VM_TOOLS)
	@cd src/q3vm/vdu && ./build.sh

