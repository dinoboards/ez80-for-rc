SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: EZ80.COM

.PHONY: EZ80.COM
EZ80.COM:
	@ez80-cc --output-dir ../bin --iquote ../common EZ80.COM ez80.c ez80_io_bus_cycles_get.asm ez80_mem_bus_cycles_get.asm ez80_cpu_freq_get.asm
