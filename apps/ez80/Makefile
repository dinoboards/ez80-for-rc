SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: EZ80.COM

.PHONY: EZ80.COM
EZ80.COM:
	@mkdir -p ../bin
	@ez80-cc --optimise size --output-dir ../bin --iquote ../common EZ80.COM \
	support_emulation.asm \
	config-io.c \
	config-mem.c \
	config-mem0.c \
	config-mem1.c \
	ez80.c \
	find-extended-memory.c \
	report-memory-timing.c \
	validate-mem-set-value.c \
	config-flash.c \
	config_set_tick_rate.c \
	test-at.asm \
	report-main-memory-performance.c \
	measure-main-memory-performance.asm \
	nanoprintf.c


#todo: fix issue where only whitespace is after command- causing output to scramble - as it assumes its a valid arg
