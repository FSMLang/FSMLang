#
# manages test suites
#
#

SUITES=full_test1 \
			 full_test2 \
			 full_test3 \
			 full_test5 \
			 lexer      \
			 parser

CLEAN_SUITES=$(addsuffix .clean, $(SUITES))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

all: $(SUITES)

clean: $(CLEAN_SUITES)

$(SUITES):
	$(MAKE) -C $@ CFLAGS="$(CFLAGS)" OUTPUT_DIR="$(OUTPUT_DIR)" runtest

$(CLEAN_SUITES):
	$(MAKE) -C $(basename $@) clean

