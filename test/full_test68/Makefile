##########################################
#
# makefile for individual test
#
#
.PHONEY: check_html

include ../../fsmrules.mk

FSM_HTML_FLAGS = --css-content-filename="../../fsmlang.css"

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC = $(shell $(FSM) -M -th $(FSM_SRC))

TARGET     = check_html

include ../test.mk

check_html: $(addsuffix .result, $(GENERATED_SRC))
	@-rm $(GENERATED_SRC)

$(addsuffix .result, $(GENERATED_SRC)): $(GENERATED_SRC)
	$(DIFF) $(basename $@) $(basename $@).canonical > $@

