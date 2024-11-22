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
	ez80.c \
	find-extended-memory.c \
	report-memory-timing.c \
	validate-mem-set-value.c
