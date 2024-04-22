##########################################
#
# common content for html tests
#
#

.PHONY: $(TESTS)

FSMS = $(shell find . -name "*.fsm" -printf "%P ")
TESTS = $(basename $(FSMS))

override FSM_FLAGS+=-th --css-content-internal=true --css-content-filename="$(OUTPUT_DIR)/../fsmlang.css"

NO_RUNTEST=TRUE
include $(OUTPUT_DIR)/../test/test.mk

include $(OUTPUT_DIR)/../fsmrules.mk

echotests:
	@echo $(TESTS)

runtest: $(TESTS) 
	@echo "all html tests pass"

$(TESTS): $(FSM) Makefile
	@$(FSM) $(FSM_FLAGS) $@.fsm
	@$(DIFF) $@.html $@.html.canonical > $@.result
	@echo $@ successful
	@rm -f $@.html $@.result > /dev/null
      
clean::
	-@rm -f *.html > /dev/null
	-@rm -f *.stderr > /dev/null
	-@rm -f *.result > /dev/null

