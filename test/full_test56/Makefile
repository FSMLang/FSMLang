##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
         -DSUB_MACHINE1_DEBUG \
         -DSUB_MACHINE2_DEBUG \
         -DSUB_MACHINE3_DEBUG \
         -DTOP_LEVEL_DEBUG \
         -I../ \
	 -Wall

FSM_FLAGS = -ts
FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC = \
	top_level.c \
	sub_machine1.c \
	sub_machine2.c \
	sub_machine3.c

SRC = $(shell find . -name "*-actions.c" -printf "%P ")

TARGET = check_top_level_c

include ../test.mk

include ../../fsmrules.mk

check_top_level_c: top_level.c
	@$(DIFF) top_level.c top_level.c.canonical > test.result
	@-rm $(GENERATED_SRC)
	@-rm $(GENERATED_SRC:.c=.h)


