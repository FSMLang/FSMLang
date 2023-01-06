##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG -DNEW_MACHINE_DEBUG -I../

FSM_FLAGS = 

FSM_SRC = flashdriver.fsm

SRC = flashdriver_actions.c 

OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o)

include ../test.mk
include ../../fsmrules.mk
