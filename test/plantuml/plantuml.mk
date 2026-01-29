##########################################
#
# common content for plantuml tests
#
#

.PHONY: $(TESTS)
.SECONDEXPANSION:

FSMS = $(shell find . -name "*.fsm" -printf "%P ")
TESTS = $(basename $(FSMS))

#override FSM_PLANTUML_FLAGS+=

NO_RUNTEST=TRUE
include $(OUTPUT_DIR)/../test/test.mk

include $(OUTPUT_DIR)/../fsmrules.mk

echotests:
	@echo $(TESTS)

runtest: $(TESTS) 
	@echo "all plantuml tests pass"

$(TESTS): $(FSM) $$@.plantuml Makefile
	@$(DIFF) $@.plantuml $@.plantuml.canonical > $@.result
	@echo $@ successful
	@rm -f $@.plantuml $@.result > /dev/null
clean::
	-@rm -f *.plantuml > /dev/null
	-@rm -f *.stderr > /dev/null
	-@rm -f *.result > /dev/null
	-@rm -f *.fsmdh  > /dev/null


