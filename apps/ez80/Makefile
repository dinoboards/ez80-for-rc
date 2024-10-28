SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: ez80.com

.PHONY: ez80.com
ez80.com:
	@ez80-cc --output-dir ../bin --iquote ../common ez80.c ez80_bus_cycles_get.asm ez80_cpu_freq_get.asm
