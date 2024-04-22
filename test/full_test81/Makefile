##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DMESSENGER_DEBUG -I../
#override CFLAGS += -DFSM_DEBUG -DTEST_FSM_DEBUG -I../

override FSM_FLAGS += -te

SRC = $(shell find . -name "*-actions.c" -printf "%P ")
SRC += test.c

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

include ../test.mk

include ../../fsmrules.mk


