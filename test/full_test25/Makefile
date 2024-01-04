##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
         -DSUB_MACHINE1_DEBUG \
         -DSUB_MACHINE2_DEBUG \
         -DSUB_MACHINE3_DEBUG \
         -DTOP_LEVEL_DEBUG \
         -I../ \
	 -Wall \
	 -ggdb

FSM_FLAGS = -ts --core-logging-only=true

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC = \
	sub_machine1.c \
	sub_machine2.c \
	sub_machine3.c

SRC = $(shell find . -name "*-actions.c" -printf "%P ")
SRC += test.c

include ../test.mk

include ../../fsmrules.mk


