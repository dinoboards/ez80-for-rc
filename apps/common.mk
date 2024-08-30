SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

# --assemble-only
ZCC := zcc +cpm -compiler=sdcc --vc -Cs --Werror -lmath32 -I../common -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1 -create-app
BIN = ../bin/

TARGETS := $(addsuffix .com,$(addprefix $(BIN),$(APPS)))

all: $(addsuffix .com,$(APPS))

%.com: $(BIN)%.com
	@

$(TARGETS):
	@mkdir -p $(BIN)
	$(ZCC) --assemble-only $(filter-out %.h,$^) -o $(notdir $@)
	mkdir -p $(BIN)$(APPS)
	mv $(patsubst %.c,%.c.asm,$(filter-out %.h,$(filter-out %.asm,$^))) $(BIN)$(APPS)
	$(ZCC)  $(filter-out %.h,$^) -o $(notdir $@)
	mv $(call uppercase,$(notdir $@)) "$@"
	rm -f *.com
	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"


.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;

clean:
	@rm -rf $(BIN)


# A macro for converting a string to uppercase
uppercase_TABLE:=a,A b,B c,C d,D e,E f,F g,G h,H i,I j,J k,K l,L m,M n,N o,O p,P q,Q r,R s,S t,T u,U v,V w,W x,X y,Y z,Z

define uppercase_internal
$(if $1,$$(subst $(firstword $1),$(call uppercase_internal,$(wordlist 2,$(words $1),$1),$2)),$2)
endef

define uppercase
$(eval uppercase_RESULT:=$(call uppercase_internal,$(uppercase_TABLE),$1))$(uppercase_RESULT)
endef
