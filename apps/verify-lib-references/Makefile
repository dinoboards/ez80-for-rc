SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: VFYADL.COM VFYADL.EXE VFYZ80.COM

.PHONY: VFYADL.COM
VFYADL.COM:
	@mkdir -p ../bin
	@ez80-cc --output-dir ../bin -DEZ80CLANG=1 --optimise size VFYADL.COM main.c test_getopt.c

.PHONY: VFYADL.EXE
VFYADL.EXE:
	@mkdir -p ../bin
	@ez80-cc --mem-model 2MB --output-dir ../bin -DEXE=1 -DEZ80CLANG=1 --optimise size VFYADL.EXE main.c test_getopt.c

include ../common.mk

# CP/M
$(BIN)VFYZ80.COM: main.c test_getopt.c
	$(compile)
