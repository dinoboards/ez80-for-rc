SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory
.DELETE_ON_ERROR:

$(shell cd ../tools && ./grab-compilers.sh )

LCC := ../tools/lcc
Q3ASM := ../tools/q3asm

INCLUDES :=  $(wildcard ../includes/*.h)
VDU_C_SOURCE := $(wildcard ./*.c)
VM_TOOLS := $(wildcard ../tools/*)
VM_C_SOURCE := $(wildcard ../host/*)
VDU_ASM_FILES := $(VDU_C_SOURCE:.c=.vmasm)

all: bytecode.qvm

%.vmasm: %.c
	@$(LCC) -I$(shell realpath -s --relative-to="$<" "includes") -DQ3_VM -S -Wf-target=bytecode -Wf-g -o $@ $<
	@echo "Compiled $<"

vdu.vmasm: vdu.c $(INCLUDES) $(VM_TOOLS) $(VM_C_SOURCE)
vdu_init.vmasm: vdu_init.c $(INCLUDES) $(VM_TOOLS) $(VM_C_SOURCE)
bg_lib.vmasm: bg_lib.c bg_lib.h $(INCLUDES) $(VM_TOOLS) $(VM_C_SOURCE)
dispatch.vmasm: dispatch.c $(INCLUDES) $(VM_TOOLS) $(VM_C_SOURCE)

bytecode.qvm: $(VDU_ASM_FILES)
	@$(Q3ASM) -v -l -m -o "bytecode" dispatch.vmasm vdu_init.vmasm vdu.vmasm bg_lib.vmasm g_syscalls
	@./import-bytecode.sh
	@echo "Assembled vdu/bytecode.qvm"

