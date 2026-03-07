##########################################
#
# makefile for individual test
#
#

.PHONEY: check_h

TARGET = check_h

FSM_FLAGS = --add-doxygen-blocks=true

include ../test.mk

include ../../fsmrules.mk

check_h: $(addsuffix .result, $(GENERATED_HDR)) clean
	@echo $@ passes

$(addsuffix .result, $(GENERATED_HDR)): $(GENERATED_HDR)
	@$(DIFF) $(basename $@) $(basename $@).canonical > $@
	@rm -f $@

