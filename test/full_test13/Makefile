##########################################
#
# makefile for individual test
#
#

override CFLAGS += -Werror -DFSM_DEBUG -DNEW_MACHINE_DEBUG -I../

DIFF_FLAGS= -I "Warning: Ignoring compact array request because actions do not return appropriately\\." 

FSM_FLAGS = -c

FSM_SRC = parser-test5.fsm

SRC = pt5-actions.c

OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o)


include ../test.mk
include ../../fsmrules.mk
