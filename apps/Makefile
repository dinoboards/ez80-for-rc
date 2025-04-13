SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory

# Define the sub directories to be built
BINARIES := VHBIOS CPMEXMPL CUBE DHRY EXE EZ80 HWADL HWZ80 I2CSCAN LINE MATRIX8 MBROT VFYADL VFYADL VFYZ80 WHET WOLF3D SUPHDMI

# Default target
all: $(BINARIES:%=$(BIN)%.COM)
	@

# Pattern rule to build each binary
.PHONY: $(BIN)%.COM
$(BIN)%.COM: common-libs
	@LOWERCASE_STEM=$(shell echo $* | tr '[:upper:]' '[:lower:]') && \
	$(MAKE) -C $${LOWERCASE_STEM}

.PHONY: bin/VFYADL.COM
bin/VFYADL.COM :
	$(MAKE) -C verify-lib-references

.PHONY: bin/VHBIOS.COM
.PHONY: bin/VHBIOS.EXE
bin/VHBIOS.COM bin/VHBIOS.EXE :
	$(MAKE) -C verify-hbios

.PHONY: bin/VFYZ80.COM
bin/VFYZ80.COM: bin/VFYADL.COM

.PHONY: common-libs
common-libs:
	@$(MAKE) -C common

z88dk:
	@docker run -w /host/${PWD} -v /:/host/ -u $(USER_ID):$(GROUP_ID) -it z88dk/z88dk

clean:
	@rm -rf $(BIN)
