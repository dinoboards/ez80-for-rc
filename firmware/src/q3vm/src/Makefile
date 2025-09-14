SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory
.DELETE_ON_ERROR:

$(shell cd ../host && ./import-vm.sh)

LCC := lcc
Q3ASM := q3asm

INCLUDES :=  $(wildcard ../includes/*.h) $(wildcard ./*.h) $(wildcard ./vdu/*.h) $(wildcard ./vdp/*.h) ../../includes/vm-shared-mem.h $(wildcard ../../includes/vdu*.h)
VDU_C_SOURCE := $(filter-out ./dispatch.c,$(wildcard ./*.c) $(wildcard ./vdu/*.c) $(wildcard ./vdp/*.c)) #$(wildcard ./stdlib/*.c)
VM_C_SOURCE := $(wildcard ../host/*)
VDU_ASM_FILES := $(VDU_C_SOURCE:.c=.vmasm)

INCLUDE_PATH := ../../../../../../../../../../../../..$(shell realpath ../includes/)
HOST_INCLUDE_PATH := ../../../../../../../../../../../$(shell realpath ../../../src/includes/)

all: ../vm_bytecode.c
	@echo > /dev/null

%.vmasm: %.c $(INCLUDES)
	@$(LCC) -A  -I$(INCLUDE_PATH) -I$(HOST_INCLUDE_PATH) -DQ3_VM -S -Wf-target=bytecode -Wf-g -o $@ $<
	@echo "Compiled $<"

bytecode.qvm: $(VDU_ASM_FILES)
	@$(Q3ASM) -l -m -o "bytecode"  $(VDU_ASM_FILES) system-calls-ids.asm host-data.asm
	@mv bytecode.h ../bytecode.h
	@clang-format -i  ../bytecode.h
	@echo "Assembled vdu/bytecode.qvm"

../vm_bytecode.c ../vm_bytecode.h: ./bytecode.qvm
	@echo "recreating bytecode"
	@./export-bytecode.sh
