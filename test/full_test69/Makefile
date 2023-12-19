##########################################
#
# makefile for individual test
#
#
.PHONEY: check_output

override CFLAGS += -DANOTHER_NEW_MACHINE_DEBUG                                               \
                   -DANOTHER_NEW_MACHINE_ANOTHER_SUB_MACHINE1_DEBUG                          \
                   -DANOTHER_NEW_MACHINE_ANOTHER_SUB_MACHINE1_ANOTHER_SUB_SUB_MACHINE1_DEBUG \
                   -DNEW_MACHINE_DEBUG                                                       \
                   -DNEW_MACHINE_SUB_MACHINE1_DEBUG                                          \
                   -DNEW_MACHINE_SUB_MACHINE1_SUB_SUB_MACHINE1_DEBUG 

FSM_FLAGS = -ts --add-event-cross-reference

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")
SRC = $(shell find . -name "*.c" -printf "%P ")

include ../../fsmrules.mk

#GENERATED_HEADERS gets created in fsmrules.mk; we
# add the submach headers for our "clean" target to work properly.
GENERATED_HEADERS += $(GENERATED_SRC:.c=_submach.h)


#TARGET     = check_objects

include ../test.mk

check_objects:
	@echo $(OBJS)
	@echo $(SRC)
	@echo $(FSM_SRC)
	@echo $(GENERATED_SRC)

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

#$(GENERATED_HEADERS):
#	@echo "looking for: " $@
#	$(FSM) $(FSM_SRC)

