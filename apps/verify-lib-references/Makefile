SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: VFYADL.COM VFYZ80.COM

.PHONY: VFYADL.COM
VFYADL.COM:
	@ez80-cc --output-dir ../bin -DEZ80CLANG=1 --optimise size VFYADL.COM main.c test_getopt.c

APPS := VFYZ80

include ../common.mk

$(BIN)$(APPS).COM: $(BIN)$(APPS)/main.c.o $(BIN)$(APPS)/test_getopt.c.o
