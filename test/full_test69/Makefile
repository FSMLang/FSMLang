##########################################
#
# makefile for individual test
#
#
.PHONEY: check_output

include ../../fsmrules.mk

FSM_FLAGS = -ts --add-event-cross-reference

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC = $(shell $(FSM) -M -tc $(FSM_SRC))
GENERATED_HEADERS = $(GENERATED_SRC:.c=.h)

#TARGET     = check_output

include ../test.mk

check_output: check_c check_h

check_c: $(addsuffix .result, $(GENERATED_SRC))
	@-rm $(GENERATED_SRC)
	@-rm fsmout

check_h: $(addsuffix .result, $(GENERATED_HEADERS))
	@-rm $(GENERATED_HEADERS)
	@-rm fsmout

$(addsuffix .result, $(GENERATED_SRC)): $(GENERATED_SRC)
	$(DIFF) $(basename $@) $(basename $@).canonical > $@
	@rm $@

$(addsuffix .result, $(GENERATED_HEADERS)): $(GENERATED_HEADERS)
	$(DIFF) $(basename $@) $(basename $@).canonical > $@
	@rm $@

