##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG -DNEW_MACHINE_DEBUG -DSUB_MACHINE1_DEBUG -I../

FSM_FLAGS = 

FSM_SRC       = $(shell find . -name "*.fsm" -printf "%P ")
SRC           = nm-actions.c sm1-actions.c
GENERATED_SRC = subMachine1.c

include ../test.mk
include ../../fsmrules.mk

subMachine1.c: full-test18.c

