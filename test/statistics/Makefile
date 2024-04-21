##########################################
#
# makefile for individual test
#
#

.PHONY: $(TESTS)

FSMS = $(shell find . -name "*.fsm" -printf "%P ")
TESTS = $(basename $(FSMS))

override FSM_FLAGS=-s
# --print-action-array=true

NO_RUNTEST=TRUE
include ../test.mk

include ../../fsmrules.mk

echotests:
	@echo $(TESTS)

runtest: $(TESTS) 
	@echo "all statistics tests pass"

$(TESTS): $(FSM) Makefile
	@$(FSM) $(FSM_FLAGS) $@.fsm > $@.out 2> $@.stderr
	@cat $@.stderr >> $@.out; rm -f $@.stderr
	@$(DIFF) $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm -f $@.out $@.result > /dev/null
      
clean::
	-@rm -f *.out > /dev/null
	-@rm -f *.stderr > /dev/null
	-@rm -f *.result > /dev/null

