##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG -DNEW_MACHINE_DEBUG -I../

FSM_SRC = parser-test6.fsm

SRC = pt6-actions.c

OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o)

include ../test.mk
include ../../fsmrules.mk
