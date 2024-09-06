SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ZCC_EXTRA := --vc -Cs --Werror

ifdef RELEASE
	BIN = ../bin/release/
  ZCC_EXTRA += -SO3 --max-allocs-per-node200000
else
	BIN = ../bin/debug/
endif
ZCC := zcc +cpm -compiler=sdcc -lm -I../common

TARGETS := $(addsuffix .com,$(addprefix $(BIN),$(APPS)))

all: $(addsuffix .com,$(APPS))

%.com: $(BIN)%.com
	@

$(TARGETS):
	@mkdir -p $(BIN)$(APPS)
	$(ZCC) $(ZCC_EXTRA) $(foreach lib,$(filter %.lib,$^),-l$(lib)) $(filter-out %.h,$(filter-out %.lib,$^)) -o $@ -create-app
	filesize=$$(stat -c%s "$@")
	echo "Compiled $(notdir $@) ($$filesize bytes) from $(notdir $(filter-out %.h,$(filter-out %.lib,$^)))"


.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;

clean:
	@rm -rf $(BIN)


define compile
	@mkdir -p $(dir $@)
	$(ZCC) $(ZCC_EXTRA) --c-code-in-asm --assemble-only $< -o $@
	echo "Compiled $(notdir $@) from $(notdir $<)"
endef

define assemble
	@mkdir -p $(dir $@)
	$(ZCC) $(ZCC_EXTRA)  --compile-only $< -o $@
	echo "Assembled $(notdir $@) from $(notdir $<)"
endef

define buildlib
	@mkdir -p $(dir $@)
	z88dk-z80asm -x$@ $<
	echo "Packaged $(notdir $@) from $(notdir $<)"
endef

%.lib: %.o; $(buildlib)
$(BIN)common/%.o: ../common/%.asm;	$(assemble)

$(BIN)%.c.o: $(BIN)%.c.asm; $(assemble)

.PRECIOUS: $(BIN)$(APPS)/%.c.asm
$(BIN)$(APPS)/%.c.asm: %.c; $(compile)

IFL_ASM_FILES := $(wildcard ../common/ifl/*.asm)
IFL_LIB_FILES := $(patsubst ../common/ifl/%.asm,$(BIN)common/ifl/%.lib,$(IFL_ASM_FILES))
IFL_LIB=$(IFL_LIB_FILES) ../common/ifl.h

EZ80_ASM_FILES := $(wildcard ../common/ez80-instr/*.asm)
EZ80_LIB_FILES := $(patsubst ../common/ez80-instr/%.asm,$(BIN)common/ez80-instr/%.lib,$(EZ80_ASM_FILES))
EZ80_LIB=$(EZ80_LIB_FILES) #../common/ez80-instr.inc
