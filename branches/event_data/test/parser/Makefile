#
# manages test suites
#
#

SUITES= $(shell find . -type d -printf "%P ")

CLEAN_SUITES=$(addsuffix .clean, $(SUITES))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

all: $(SUITES)

clean: $(CLEAN_SUITES)

$(SUITES):
	$(MAKE) -C $@ CFLAGS="$(CFLAGS)" OUTPUT_DIR="$(OUTPUT_DIR)" runtest

$(CLEAN_SUITES):
	$(MAKE) -C $(basename $@) clean

