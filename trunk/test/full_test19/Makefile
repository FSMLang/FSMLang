##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
         -DSUB__MACHINE1_DEBUG \
         -DSUB__MACHINE2_DEBUG \
         -DSUB__MACHINE3_DEBUG \
         -DTOP__LEVEL_DEBUG \
         -I../

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC = \
	sub_machine1.c \
	sub_machine2.c \
	sub_machine3.c

SRC = $(shell find . -name "*-actions.c" -printf "%P ")

include ../test.mk

include ../../fsmrules.mk


