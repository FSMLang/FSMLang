##########################################
#
# makefile for individual test
#
#

.PHONEY: check_c

override CFLAGS += -DFSM_DEBUG \
         -DSUB_MACHINE1_DEBUG \
         -DSUB_MACHINE2_DEBUG \
         -DSUB_MACHINE3_DEBUG \
         -DTOP_LEVEL_DEBUG \
         -I../ \
	 		-Wall

FSM_FLAGS     = --short-user-fn-names -ts
FSM_SRC       = $(shell find . -name "*.fsm" -printf "%P ")

TARGET = check_c

include ../test.mk

include ../../fsmrules.mk

check_c: $(addsuffix .result, $(GENERATED_SRC))
	@-rm -f $(GENERATED_SRC)
	@-rm -f $(GENERATED_HDR)
	@-rm -f fsmout

$(addsuffix .result, $(GENERATED_SRC)): $(GENERATED_SRC)
	$(DIFF) $(basename $@) $(basename $@).canonical > $@
	@rm -f $@

