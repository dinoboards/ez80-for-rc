BIN=./bin/

all:
	@docker run -v .:/src/ -it z88dk/z88dk make apps -B -j


apps: $(BIN)matrix8.com $(BIN)i2cscan.com $(BIN)dhry.com $(BIN)whet.com

format:
	@$(MAKE) -C i2cscan format --no-print-directory
	@$(MAKE) -C matrix8 format --no-print-directory

clean:
	rm -rf $(BIN)

.PHONY: $(BIN)i2cscan.com
$(BIN)i2cscan.com:
	@$(MAKE) -C i2cscan --no-print-directory

.PHONY: $(BIN)matrix8.com
$(BIN)matrix8.com:
	@$(MAKE) -C matrix8 --no-print-directory

.PHONY: $(BIN)dhry.com
$(BIN)dhry.com:
	@$(MAKE) -C dhry --no-print-directory

.PHONY: $(BIN)whet.com
$(BIN)whet.com:
	@$(MAKE) -C whet --no-print-directory
