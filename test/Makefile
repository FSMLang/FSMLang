#
# manages test suites
#
#
 
SUITES = lexer parser unit statistics html
SUITES += $(shell find . -maxdepth 1 -type d -name "full_test*" -printf "%P ")

CLEAN_SUITES=$(addsuffix .clean, $(SUITES))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

all: $(SUITES)

clean: $(CLEAN_SUITES)

$(SUITES):
	$(MAKE) -C $@ CFLAGS="$(CFLAGS)" ARCH="$(ARCH)" OUTPUT_DIR="$(OUTPUT_DIR)" runtest

$(CLEAN_SUITES):
	$(MAKE) -C $(basename $@) clean

check:
	@echo $(SUITES)
