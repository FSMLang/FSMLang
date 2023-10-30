##########################################
#
# makefile for individual test
#
#
.PHONEY: check_c

include ../../fsmrules.mk

FSM_FLAGS = --add-event-cross-reference

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC = $(shell $(FSM) -M -tc $(FSM_SRC))

TARGET     = check_c

include ../test.mk

check_c: $(addsuffix .result, $(GENERATED_SRC))
	@-rm $(GENERATED_SRC)
	@-rm fsmout

$(addsuffix .result, $(GENERATED_SRC)): $(GENERATED_SRC)
	$(DIFF) $(basename $@) $(basename $@).canonical > $@
	@rm $@

