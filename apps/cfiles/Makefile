SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: CFILES.EXE CFILES.COM CFILESZ.COM

.PHONY: CFILES.COM
CFILES.COM:
	@ez80-cc --output-dir ../bin --optimise size CFILES.COM cfiles.c


.PHONY: CFILES.EXE
CFILES.EXE:
	@ez80-cc --mem-model 2MB  --output-dir ../bin --optimise size CFILES.EXE cfiles.c


APPS := CFILESZ

include ../common.mk

$(BIN)$(APPS).COM: $(BIN)$(APPS)/cfiles.c.o
