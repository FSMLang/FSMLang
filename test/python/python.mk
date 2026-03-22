##########################################
#
# common content for py tests
#
#

.PHONY: $(TESTS)
.SECONDEXPANSION:

FSMS = $(shell find . -name "*.fsm" -printf "%P ")
TESTS = $(basename $(FSMS))

NO_RUNTEST=TRUE
include $(OUTPUT_DIR)/../test/test.mk

include $(OUTPUT_DIR)/../fsmrules.mk

echotests:
	@echo $(TESTS)

runtest: $(TESTS) 
	@echo "all python tests pass"
	@$(MAKE) clean

$(TESTS): $(FSM) $$@.py Makefile
	@$(DIFF) $@.py $@.py.canonical > $@.result
	@echo $@ successful
	@rm -f $@.py $@.result > /dev/null

clean::
	-@rm -f *.py > /dev/null
	-@rm -f *.stderr > /dev/null
	-@rm -f *.result > /dev/null
	-@rm -f *.fsmdh  > /dev/null
	-@rm -f fsmout  > /dev/null


