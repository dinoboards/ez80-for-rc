SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: EZ80.COM

.PHONY: EZ80.COM
EZ80.COM:
	@ez80-cc --optimise speed --output-dir ../bin --iquote ../common EZ80.COM \
	config-mem.c \
	config-mem0.c \
	ez80_cpu_freq_get.asm \
	ez80_io_bus_cycles_get.asm \
	ez80_mem_bus_cycles_get.asm \
	ez80_mem_bus_cycles_set.asm \
	ez80_mem0_bus_cycles_set.asm \
	ez80.c \
	find-extended-memory.c \
	report-memory-timing.c \
	validate-mem-set-value.c
