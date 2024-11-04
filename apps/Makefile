SHELL := /bin/sh
BIN=./bin/
MAKEFLAGS += --no-print-directory

# Define the binaries
BINARIES := CUBE EZ80 HELLO-WORLD-Z80 HELLO-WORLD-ADL MBROT LINE MATRIX8 I2CSCAN DHRY WHET FWUPDATE FWMAIN FWSTATUS

# Default target
all: $(BINARIES:%=$(BIN)%.COM)
	@

# Pattern rule to build each binary
.PHONY: $(BIN)%.COM
$(BIN)%.COM: common-libs
	@LOWERCASE_STEM=$(shell echo $* | tr '[:upper:]' '[:lower:]') && \
	$(MAKE) -C $${LOWERCASE_STEM}

.PHONY: common-libs
common-libs:
	@$(MAKE) -C common

z88dk:
	@docker run -w /host/${PWD} -v /:/host/ -u $(USER_ID):$(GROUP_ID) -it z88dk/z88dk

clean:
	@rm -rf $(BIN)
