##########################################
#
# makefile for individual test
#
#
.PHONEY: check_plantuml

include ../../fsmrules.mk

FSM_PLANTUML_FLAGS = --add-plantuml-title --add-plantuml-legend="top,right" --exclude-actions-from-plantuml-legend

FSM_SRC := $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC = $(shell $(FSM) -M -tp $(FSM_SRC))

TARGET     = check_plantuml

include ../test.mk

check_plantuml: $(addsuffix .result, $(GENERATED_SRC))
	@-rm *.result
	@-rm fsmout
	@-rm $(GENERATED_SRC)

$(addsuffix .result, $(GENERATED_SRC)): $(GENERATED_SRC)
	@$(DIFF) $(basename $@) $(basename $@).canonical > $@


