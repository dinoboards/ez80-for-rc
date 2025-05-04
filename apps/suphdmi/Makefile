SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: SUPHDMI.COM V99X8.COM

.PHONY: SUPHDMI.COM
SUPHDMI.COM:
	@mkdir -p ../bin
	@ez80-cc -DSUPHDMI --optimise speed --lib v99x8-hdmi --output-dir ../bin SUPHDMI.COM suphdmi.c gr_7_test_pattern.c gr_6_test_pattern.c gr_5_test_pattern.c sup_test_pattern.c test_pattern.c large_palette.c wait_for_key.c

.PHONY: V99X8.COM
V99X8.COM:
	@mkdir -p ../bin
	@ez80-cc -DV99X8 --optimise speed --lib v99x8-standard --output-dir ../bin V99X8.COM suphdmi.c gr_7_test_pattern.c gr_6_test_pattern.c gr_5_test_pattern.c sup_test_pattern.c test_pattern.c large_palette.c wait_for_key.c
