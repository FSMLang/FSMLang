##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
         -DSUB_MACHINE1_DEBUG \
         -DSUB_MACHINE2_DEBUG \
         -DNEW_MACHINE_DEBUG \
         -I../ \
	 -Wall \
	 -ggdb

override FSM_FLAGS += --short-user-fn-names --generate-run-function=true -ts

SRC = $(shell find . -name "*-actions.c" -printf "%P ")
SRC += test.c

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

include ../../fsmrules.mk

include ../test.mk

