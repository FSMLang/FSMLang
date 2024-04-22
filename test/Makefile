#
# manages test suites
#
#
 
SUITES = lexer parser unit statistics html
FULL_TESTS = $(shell find . -maxdepth 1 -type d -name "full_test*" -printf "%P ")

SUITES += $(FULL_TESTS)

CLEAN_SUITES=$(addsuffix .clean, $(SUITES))
STAT_FULL_TESTS=$(addsuffix .stats, $(FULL_TESTS))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

all: $(SUITES)

clean: $(CLEAN_SUITES)
	-@rm stats.txt > /dev/null

stats: $(STAT_FULL_TESTS)

$(SUITES):
	$(MAKE) -C $@ CFLAGS="$(CFLAGS)" ARCH="$(ARCH)" OUTPUT_DIR="$(OUTPUT_DIR)" runtest

$(CLEAN_SUITES):
	$(MAKE) -C $(basename $@) clean

$(STAT_FULL_TESTS):
	-@$(MAKE) -C $(basename $@) stats.txt > /dev/null
	-@echo $(basename $@) >> stats.txt
	-@grep TARGET $(basename $@)/Makefile >> stats.txt
	-@grep FSM_FLAGS $(basename $@)/Makefile >> stats.txt
	-@grep FSM_HTML_FLAGS $(basename $@)/Makefile >> stats.txt
	-@grep FSM_PLANTUML_FLAGS $(basename $@)/Makefile >> stats.txt
	-@cat $(basename $@)/stats.txt >> stats.txt

check:
	@echo $(SUITES)
