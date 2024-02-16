##########################################
#
# makefile for individual test
#
#

include ../../fsmrules.mk

override CFLAGS += -DFSM_DEBUG \
         -DSUB_MACHINE1_DEBUG \
         -DSUB_MACHINE2_DEBUG \
         -DSUB_MACHINE3_DEBUG \
         -DTOP_LEVEL_DEBUG \
         -I../ \
	 -Wall \
	 -ggdb

FSM_FLAGS = -ts --generate-weak-fns=false

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

TARGET=fail_is_pass

GENERATED_SRC = $(shell $(FSM) -M -ts $(FSM_SRC))
GENERATED_HDR = $(shell $(FSM) -Mh -ts $(FSM_SRC))

SRC = $(shell find . -name "*-actions.c" -printf "%P ")
SRC += test.c

include ../test.mk

