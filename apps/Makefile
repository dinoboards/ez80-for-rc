# To make all apps - run  docker run --platform linux/amd64 -v .:/src/ -it z88dk/z88dk make

BIN=./bin/

all:
	@docker run --platform linux/amd64 -v .:/src/ -it z88dk/z88dk make apps -B

apps: $(BIN)matrix8.com $(BIN)i2cscan.com

format:
	@$(MAKE) -C i2cscan format --no-print-directory
	@$(MAKE) -C matrix8 format --no-print-directory

clean:
	rm -rf $(BIN)


$(BIN)i2cscan.com:
	@printf "i2cscan: "
	@$(MAKE) -C i2cscan --no-print-directory

$(BIN)matrix8.com:
	@printf "matrix8: "
	@$(MAKE) -C matrix8 --no-print-directory
