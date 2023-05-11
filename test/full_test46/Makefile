##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG -DNEW_MACHINE_DEBUG -I../

SRC = $(shell find . -name "*-actions.c" -printf "%P ")
FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC=subMachine1.c \
              subMachine2.c

include ../test.mk
include ../../fsmrules.mk
