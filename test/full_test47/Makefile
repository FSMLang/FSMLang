##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG          \
                   -DNEW_MACHINE_DEBUG  \
                   -DSUB_MACHINE1_DEBUG \
                   -DSUB_MACHINE2_DEBUG \
                   -I../

override FSM_FLAGS += -ts                  \
                   --generate-run-function 

SRC = $(shell find . -maxdepth 1 -name "*-actions.c" -printf "%P ")
FSM_SRC = $(shell find . -maxdepth 1 -name "*.fsm" -printf "%P ")

GENERATED_SRC=subMachine1.c \
              subMachine2.c

include ../test.mk
include ../../fsmrules.mk

look_at_source:
	echo $(SRC)
	echo $(FSM_SRC)
