##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DMESSENGER_DEBUG -I../
override CFLAGS += -DFSM_DEBUG -DTEST_FSM_DEBUG -I../

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

SRC = $(shell find . -name "*-actions.c" -printf "%P ")
SRC += test.c

include ../test.mk

include ../../fsmrules.mk


