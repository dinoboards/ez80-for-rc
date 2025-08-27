SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory
.DELETE_ON_ERROR:

$(shell cd ../../../q3vm/host && ./import-vm.sh )

LCC := ../../../q3vm/tools/lcc
Q3ASM := ../../../q3vm/tools/q3asm

INCLUDES :=  $(wildcard ../../../q3vm/includes/*.h)
VDU_C_SOURCE := $(wildcard ./*.c)
VM_C_SOURCE := $(wildcard ../../../a3vm/host/*)
VDU_ASM_FILES := $(VDU_C_SOURCE:.c=.vmasm)

INCLUDE_PATH := ../../../../../../../../../../../../../../../../../../../../$(shell realpath ../../../q3vm/includes/)

all: ../vdu_vm_bytecode.c
	@echo > /dev/null

%.vmasm: %.c
	@$(LCC) -I$(INCLUDE_PATH) -DQ3_VM -S -Wf-target=bytecode -Wf-g -o $@ $<
	@echo "Compiled $<"

vdu.vmasm: vdu.c $(INCLUDES)  $(VM_C_SOURCE)
vdu_init.vmasm: vdu_init.c $(INCLUDES)  $(VM_C_SOURCE)
bg_lib.vmasm: bg_lib.c bg_lib.h $(INCLUDES)  $(VM_C_SOURCE)
dispatch.vmasm: dispatch.c $(INCLUDES)  $(VM_C_SOURCE)

bytecode.qvm: $(VDU_ASM_FILES)
	@$(Q3ASM) -v -l -m -o "bytecode" dispatch.vmasm vdu_init.vmasm vdu.vmasm bg_lib.vmasm g_syscalls
	@echo "Assembled vdu/bytecode.qvm"

../vdu_vm_bytecode.c ../vdu_vm_bytecode.h: bytecode.qvm
	@./export-bytecode.sh
