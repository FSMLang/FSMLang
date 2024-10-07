##########################################
#
# makefile for individual test
#
#
.PHONEY: check_html

include ../../fsmrules.mk
FSM_RULES_INCLUDED=true

FSM_HTML_FLAGS = --css-content-filename="../../fsmlang.css"

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC = $(shell $(FSM) -M -th $(FSM_SRC))

TARGET     = check_html

include ../test.mk

check_html: $(addsuffix .result, $(GENERATED_SRC))
	@-rm -f $(GENERATED_SRC)
	@-rm -f fsmout

$(addsuffix .result, $(GENERATED_SRC)): $(GENERATED_SRC)
	$(DIFF) $(basename $@) $(basename $@).canonical > $@
	@rm -f $@

