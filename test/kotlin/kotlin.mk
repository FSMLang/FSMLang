##########################################
#
# common content for kotlin tests
#
#

.PHONY: $(TESTS)
.SECONDEXPANSION:

FSMS = $(shell find . -name "*.fsm" -printf "%P ")
TESTS = $(basename $(FSMS))

NO_RUNTEST=TRUE
include $(OUTPUT_DIR)/../test/test.mk

FSM ?= $(OUTPUT_DIR)/fsm

echotests:
	@echo $(TESTS)

runtest: $(TESTS)
	@echo "all kotlin tests pass"
	@$(MAKE) clean

$(TESTS): $(FSM) $$@.kt Makefile
	@$(DIFF) $@.kt $@.kt.canonical > $@.result
	@echo $@ successful
	@rm -f $@.kt $@.result > /dev/null

%.kt: %.fsm $(FSM)
	$(FSM) -tk $(FSM_FLAGS) $< > fsmout 2>&1

clean::
	-@rm -f *.kt > /dev/null
	-@rm -f *.stderr > /dev/null
	-@rm -f *.result > /dev/null
	-@rm -f *.fsmdh  > /dev/null
	-@rm -f fsmout  > /dev/null
