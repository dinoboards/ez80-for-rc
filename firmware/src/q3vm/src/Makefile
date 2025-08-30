SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory
.DELETE_ON_ERROR:

$(shell cd ../host && ./import-vm.sh)

LCC := lcc
Q3ASM := q3asm

INCLUDES :=  $(wildcard ../includes/*.h) $(wildcard ./*.h) $(wildcard ./vdu/*.h) $(wildcard ./vdp/*.h)
VDU_C_SOURCE := $(filter-out ./dispatch.c,$(wildcard ./*.c) $(wildcard ./vdu/*.c) $(wildcard ./vdp/*.c)) $(wildcard ./stdlib/*.c)
VM_C_SOURCE := $(wildcard ../host/*)
VDU_ASM_FILES := $(VDU_C_SOURCE:.c=.vmasm)

INCLUDE_PATH := ../../../../../../../../../../../../..$(shell realpath ../includes/)

all: ../vdu_vm_bytecode.c
	@echo > /dev/null

%.vmasm: %.c $(INCLUDES)
	@$(LCC) -A -I$(INCLUDE_PATH) -DQ3_VM -S -Wf-target=bytecode -Wf-g -o $@ $<
	@echo "Compiled $<"


bytecode.qvm: $(VDU_ASM_FILES) $(wildcard ./vdu/vdu_25/*.c)
	@$(Q3ASM) -v -l -m -o "bytecode"  dispatch.vmasm $(VDU_ASM_FILES) system-calls-ids.asm
	@echo "Assembled vdu/bytecode.qvm"

../vdu_vm_bytecode.c ../vdu_vm_bytecode.h: bytecode.qvm
	@./export-bytecode.sh
