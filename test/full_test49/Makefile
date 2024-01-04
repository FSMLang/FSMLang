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

override FSM_FLAGS += --generate-run-function

SRC = $(shell find . -name "*-actions.c" -printf "%P ")
SRC += test.c
FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC=subMachine1.c \
              subMachine2.c

include ../test.mk
include ../../fsmrules.mk
