#
# manages test suites
#
#

ifdef CFG
include $(OUTPUT_DIR)/../cfg-$(CFG).mk
endif
 
SUITES = lexer parser unit statistics html
FULL_TESTS = $(shell find . -maxdepth 1 -type d -name "full_test*" -printf "%P ")
ifneq (,$(findstring SUPPORT_INCLUDE_FILES, $(LEXER_FEATURES)))
FULL_TESTS += $(shell find . -maxdepth 1 -type d -name "v2_full_test*" -printf "%P ")
endif

SUITES += $(FULL_TESTS)
DONE = $(shell cat done)
TODO = $(filter-out $(DONE), $(SUITES))

CLEAN_SUITES=$(addsuffix .clean, $(SUITES))
STAT_FULL_TESTS=$(addsuffix .stats, $(FULL_TESTS))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

all: $(TODO)
	-@rm done      2> /dev/null

clean: $(CLEAN_SUITES)
	-@rm stats.txt 2> /dev/null
	-@rm done      2> /dev/null
	-@rm stat_done 2> /dev/null

stats: $(STAT_FULL_TESTS)

$(SUITES):
	$(MAKE) -C $@ CFG=$(CFG) CFLAGS="$(CFLAGS)" ARCH="$(ARCH)" OUTPUT_DIR="$(OUTPUT_DIR)" runtest
	@echo -n "$@ " >> done

full_tests: $(FULL_TESTS)
	@echo -n "$@ " >> done

$(CLEAN_SUITES):
	$(MAKE) -C $(basename $@) CFG=$(CFG) OUTPUT_DIR="$(OUTPUT_DIR)" clean

$(STAT_FULL_TESTS):
	-@$(MAKE) -C $(basename $@) CFG=$(CFG) stats.txt > /dev/null
	-@echo $(basename $@) >> stats.txt
	-@grep TARGET $(basename $@)/Makefile >> stats.txt
	-@grep FSM_FLAGS $(basename $@)/Makefile >> stats.txt
	-@grep FSM_HTML_FLAGS $(basename $@)/Makefile >> stats.txt
	-@grep FSM_PLANTUML_FLAGS $(basename $@)/Makefile >> stats.txt
	-@cat $(basename $@)/stats.txt >> stats.txt
	@echo -n "$@ " >> stat_done

check:
	@echo $(SUITES)
