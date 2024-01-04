##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG -DNEW_MACHINE_DEBUG -I../

FSM_SRC = parser-test12.fsm

SRC = pt12-actions.c 
SRC += test.c

OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o)

include ../test.mk
include ../../fsmrules.mk
