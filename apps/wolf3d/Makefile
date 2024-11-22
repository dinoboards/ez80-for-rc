SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: WOLF3D.EXE

# Find all .c files in the current directory
SRC_FILES := $(wildcard *.c)

.PHONY: WOLF3D.EXE
WOLF3D.EXE:
	@ez80-cc --mem-model 2MB --output-dir ../bin --optimise size WOLF3D.EXE $(SRC_FILES)
