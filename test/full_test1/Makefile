##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DTEST_FSM_DEBUG -I../
#override CFLAGS += -DFSM_DEBUG -DTEST_FSM_DEBUG -I../

FSM_SRC = parser-test2.fsm

SRC = pt2-actions.c test.c

include ../test.mk

include ../../fsmrules.mk


