#
# manages test suites
#
#
 
FULL_TESTS = $(shell find . -maxdepth 1 -type d -name "full_test*" -printf "%P ")

#
# This conditional block allows for the setting of a shorter list of suites in relevant.lst
#
ifneq ("$(wildcard relevant.lst)", "")

	SUITES = $(shell cat relevant.lst)

else

	SUITES = lexer parser unit statistics html
	SUITES += $(FULL_TESTS)

endif

STAT_FULL_TESTS=$(addsuffix .stats, $(FULL_TESTS))
CLEAN_SUITES=$(addsuffix .clean, $(SUITES))
DONE = $(shell if [ -f done ]; then cat done; fi)
TODO = $(filter-out $(DONE), $(SUITES))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

$(FULL_TESTS): override CFLAGS+=-Werror

all: $(TODO)
	-@rm done      2> /dev/null

clean: $(CLEAN_SUITES)
	-@rm stats.txt 2> /dev/null
	-@rm done      2> /dev/null
	-@rm stat_done 2> /dev/null

stats: $(STAT_FULL_TESTS)

$(SUITES):
	$(MAKE) -C $@ CFLAGS="$(CFLAGS)" ARCH="$(ARCH)" OUTPUT_DIR="$(OUTPUT_DIR)" runtest
	@echo -n "$@ " >> done

$(CLEAN_SUITES):
	$(MAKE) -j -C $(basename $@) OUTPUT_DIR="$(OUTPUT_DIR)" clean

$(STAT_FULL_TESTS):
	-@$(MAKE) -C $(basename $@) stats.txt > /dev/null
	-@echo $(basename $@) >> stats.txt
	-@grep TARGET $(basename $@)/Makefile >> stats.txt
	-@grep FSM_FLAGS $(basename $@)/Makefile >> stats.txt
	-@grep FSM_HTML_FLAGS $(basename $@)/Makefile >> stats.txt
	-@grep FSM_PLANTUML_FLAGS $(basename $@)/Makefile >> stats.txt
	-@cat $(basename $@)/stats.txt >> stats.txt
	@echo -n "$@ " >> stat_done

check:
	@echo $(SUITES)

