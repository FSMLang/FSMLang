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

TARGET = check_top_level_h

include ../test.mk
include ../../fsmrules.mk

check_top_level_h: top_level.h
	$(DIFF) top_level.h top_level.h.canonical > test.result
	@rm test.result
	@rm *.[ch]

