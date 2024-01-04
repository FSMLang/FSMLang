##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG -DNEW_MACHINE_DEBUG -I../

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")
SRC     = $(shell find . -name "*-actions.c" -printf "%P ")
SRC     += test.c
GENERATED_SRC= subMachine1.c \
               subMachine2.c


include ../test.mk
include ../../fsmrules.mk
