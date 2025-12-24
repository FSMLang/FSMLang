##########################################
#
# makefile for individual test
#
#

CFLAGS += -Werror -DFSM_DEBUG -DTEST_FSM_DEBUG -I../

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

#action functions are placed in files ending in "-actions.c"
SRC += $(shell find . -name "*-actions.c" -printf "%P ")

#some tests have main in one of the action fiies; others put it in test.c
SRC += $(shell find . -name "test.c" -print)

include ../../fsmrules.mk

include ../test.mk


