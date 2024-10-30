SHELL := /bin/sh
SHELLFLAGS := -c -e -x
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ZCC_EXTRA := --vc -Cs --Werror

USER_ID := $(shell id -u ${USER})
GROUP_ID := $(shell id -g ${USER})
Z88DK_DOCKER := docker run -w /host/$${PWD} -v /:/host/ -u $(USER_ID):$(GROUP_ID) -t z88dk/z88dk
Z88DK_DOCKER_MAKE := $(Z88DK_DOCKER)

# ZCC_EXTRA += -SO3 --max-allocs-per-node200000
BIN = ../bin/
ZCC := $(Z88DK_DOCKER_MAKE) zcc +cpm -compiler=sdcc -lm -I../common -SO3 --max-allocs-per-node200000

TARGETS := $(addsuffix .COM,$(addprefix $(BIN),$(APPS)))

all: $(addsuffix .COM,$(APPS))

%.COM: $(BIN)%.COM
	@echo > /dev/null

$(TARGETS):
	@set -e
	mkdir -p $(BIN)$(APPS)
	$(ZCC) $(ZCC_EXTRA) $(foreach lib,$(filter %.lib,$^),-l$(lib)) $(filter-out %.inc,$(filter-out %.h,$(filter-out %.lib,$^))) -o $@ -create-app
	filesize=$$(stat -c%s "$@")
	rm -f $(BIN)$(APPS)_CODE.bin
	echo "Compiled $(notdir $@) ($$filesize bytes) from $(notdir $(filter-out %.h,$(filter-out %.lib,$^)))"

clean:
	@rm -rf $(BIN)

define compile
	@set -e
	@mkdir -p $(dir $@)
	$(ZCC) $(ZCC_EXTRA) --c-code-in-asm --assemble-only $< -o $@
	echo "Compiled $(notdir $@) from $(notdir $<)"
endef

define assemble
	@set -e
	@mkdir -p $(dir $@)
	$(ZCC) $(ZCC_EXTRA)  --compile-only $< -o $@
	echo "Assembled $(notdir $@) from $(notdir $<)"
endef

define buildlib
	@set -e
	@mkdir -p $(dir $@)
	$(Z88DK_DOCKER_MAKE) z88dk-z80asm -x$@ $<
	echo "Packaged $(notdir $@) from $(notdir $<)"
endef

.PRECIOUS: %.lib
%.lib: %.o; $(buildlib)

.PRECIOUS: $(BIN)common/%.o
$(BIN)common/%.o: ../common/%.asm;	$(assemble)

.PRECIOUS: $(BIN)%.c.o
$(BIN)%.c.o: $(BIN)%.c.asm; $(assemble)

.PRECIOUS: $(BIN)$(APPS)/%.c.asm
$(BIN)$(APPS)/%.c.asm: %.c; $(compile)

IFL_ASM_FILES := $(wildcard ../common/ifl/*.asm)
IFL_LIB_FILES := $(patsubst ../common/ifl/%.asm,$(BIN)common/ifl/%.lib,$(IFL_ASM_FILES))
IFL_LIB=$(IFL_LIB_FILES) ../common/ifl.h

EZ80_ASM_FILES := $(wildcard ../common/ez80-instr/*.asm)
EZ80_O_FILES := $(patsubst ../common/ez80-instr/%.asm,$(BIN)common/ez80-instr/%.o,$(EZ80_ASM_FILES))
EZ80_LIB_FILES := $(patsubst ../common/ez80-instr/%.asm,$(BIN)common/ez80-instr/%.lib,$(EZ80_ASM_FILES))
EZ80_LIB=$(EZ80_LIB_FILES) #../common/ez80-instr.inc

EZ80_FIRMWARE_ASM_FILES := $(wildcard ../common/ez80-firmware/*.asm)
EZ80_FIRMWARE_O_FILES := $(patsubst ../common/ez80-firmware/%.asm,$(BIN)common/ez80-firmware/%.o,$(EZ80_FIRMWARE_ASM_FILES))
EZ80_FIRMWARE_LIB_FILES := $(patsubst ../common/ez80-firmware/%.asm,$(BIN)common/ez80-firmware/%.lib,$(EZ80_FIRMWARE_ASM_FILES))
EZ80_FIRMWARE_LIB=$(EZ80_FIRMWARE_LIB_FILES) #../common/ez80-instr.inc

HBIOS_LIB=$(BIN)common/hbios_sysget_tick.lib

I2C_LIB=$(BIN)common/i2c.lib ../common/i2c.h

$(EZ80_O_FILES) $(EZ80_FIRMWARE_O_FILES): ../common/ez80-instr.inc
