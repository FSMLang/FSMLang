##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
                   -DNEW_MACHINE_DEBUG \
                   -DSUB_MACHINE1_DEBUG \
                   -DSUB_MACHINE2_DEBUG \
                   -I../

override FSM_FLAGS += --add-event-cross-reference=true


FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")
GENERATED_SRC = subMachine1.c   \
                subMachine2.c   

TARGET = check_top_level_events_h

include ../test.mk
include ../../fsmrules.mk

check_top_level_events_h: top_level_events.h
	$(DIFF) top_level_events.h top_level_events.h.canonical > test.result
	-@rm -f test.result fsmout
	-@rm -f $(FSM_SRC:.fsm=.c)
	-@rm -f $(FSM_SRC:.fsm=.h)
	-@rm -f $(FSM_SRC:.fsm=_priv.h)
	-@rm -f $(FSM_SRC:.fsm=_submach.h)
	-@rm -f $(GENERATED_SRC)
	-@rm -f $(GENERATED_SRC:.c=.h)
	-@rm -f $(GENERATED_SRC:.c=_priv.h)
