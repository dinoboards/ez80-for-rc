SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: SUPHDMI.COM SUPHDMI.EXE

.PHONY: SUPHDMI.COM
SUPHDMI.COM:
	@mkdir -p ../bin
	@ez80-cc --optimise speed --lib v99x8-hdmi --output-dir ../bin SUPHDMI.COM suphdmi.c gr_7_test_pattern.c gr_6_test_pattern.c gr_5_test_pattern.c sup_test_pattern.c test_pattern.c large_palette.c wait_for_key.c

.PHONY: SUPHDMI.EXE
SUPHDMI.COM:
	@mkdir -p ../bin
	@ez80-cc -DMEM_MODEL=2MB --mem-model 2MB --optimise speed --lib v99x8-hdmi --output-dir ../bin SUPHDMI.EXE suphdmi.c gr_7_test_pattern.c gr_6_test_pattern.c gr_5_test_pattern.c sup_test_pattern.c test_pattern.c large_palette.c wait_for_key.c
