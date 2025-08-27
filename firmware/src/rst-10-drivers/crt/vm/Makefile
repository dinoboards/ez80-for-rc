SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory
.DELETE_ON_ERROR:

$(shell cd ../../../q3vm/host && ./import-vm.sh )

LCC := lcc
Q3ASM := q3asm

INCLUDES :=  $(wildcard ../../../q3vm/includes/*.h)
VDU_C_SOURCE := $(wildcard ./*.c) $(wildcard ./vdu/*.c) $(wildcard ./vdp/*.c)
VM_C_SOURCE := $(wildcard ../../../a3vm/host/*)
VDU_ASM_FILES := $(VDU_C_SOURCE:.c=.vmasm)

INCLUDE_PATH := ../../../../../../../../../../../../../../../../../../../../$(shell realpath ../../../q3vm/includes/)

all: ../vdu_vm_bytecode.c
	@echo > /dev/null

%.vmasm: %.c
	@$(LCC) -I$(INCLUDE_PATH) -DQ3_VM -S -Wf-target=bytecode -Wf-g -o $@ $<
	@echo "Compiled $<"

vdu/%.vmasm: vdu/%.c
	@$(LCC) -I$(INCLUDE_PATH) -DQ3_VM -S -Wf-target=bytecode -Wf-g -o $@ $<
	@echo "Compiled $<"

vdu.vmasm: vdu.c $(INCLUDES)  $(VM_C_SOURCE)
vdu_init.vmasm: vdu_init.c $(INCLUDES)  $(VM_C_SOURCE)
bg_lib.vmasm: bg_lib.c bg_lib.h $(INCLUDES)  $(VM_C_SOURCE)
dispatch.vmasm: dispatch.c $(INCLUDES)  $(VM_C_SOURCE)
vdu/variables.vmasm: vdu/variables.c $(INCLUDES)  $(VM_C_SOURCE)

bytecode.qvm: $(VDU_ASM_FILES) $(wildcard ./vdu/vdu_25/*.c)
	@$(Q3ASM) -v -l -m -o "bytecode"  dispatch.vmasm bg_lib.vmasm vdu_init.vmasm vdu.vmasm g_syscalls vdu/variables.vmasm vdu/vdu_25.vmasm vdu/logic_to_physical_coords.vmasm vdu/point.vmasm vdu/vdu_not_implemented.vmasm vdp/vdp_draw_line.vmasm \
	vdp/vdp_get_screen_width.vmasm vdp/vdp_get_screen_height.vmasm vdp/v99x8.vmasm
	@echo "Assembled vdu/bytecode.qvm"

../vdu_vm_bytecode.c ../vdu_vm_bytecode.h: bytecode.qvm
	@./export-bytecode.sh
