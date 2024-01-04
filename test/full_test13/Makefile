##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG -DNEW_MACHINE_DEBUG -I../

FSM_FLAGS = -c

FSM_SRC = parser-test5.fsm

SRC = pt5-actions.c

OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o)


include ../test.mk
include ../../fsmrules.mk
