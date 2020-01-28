#
# manages test suites
#
#

SUITES= lexer       \
			 	parser      \
				full_test1  \
			 	full_test2  \
			 	full_test3  \
			 	full_test5  \
			 	full_test6  \
			 	full_test7  \
			 	full_test8  \
			 	full_test9  \
			 	full_test10 \
			 	full_test11 \
			 	full_test12 \
			 	full_test13

CLEAN_SUITES=$(addsuffix .clean, $(SUITES))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

all: $(SUITES)

clean: $(CLEAN_SUITES)

$(SUITES):
	$(MAKE) -C $@ CFLAGS="$(CFLAGS)" OUTPUT_DIR="$(OUTPUT_DIR)" runtest

$(CLEAN_SUITES):
	$(MAKE) -C $(basename $@) clean

