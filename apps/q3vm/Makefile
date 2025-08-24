SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: Q3VMTEST.EXE # Q3VM.EXE

# .PHONY: Q3VM.EXE
# Q3VM.EXE:
# 	@mkdir -p ../bin
# 	@ez80-cc -DMEM_MODEL=2MB --mem-model 2MB --optimise speed --output-dir ../bin Q3VM.EXE q3vm.c vm_bytecode.c vm.asm

.PHONY: Q3VMTEST.EXE
Q3VMTEST.EXE:
	@mkdir -p ../bin
	@ez80-cc -DMEM_MODEL=2MB --mem-model 2MB --optimise speed --output-dir ../bin Q3VMTEST.EXE q3vmtest.c tests-byte-code.c vm.asm

