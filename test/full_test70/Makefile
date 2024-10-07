##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
         -DSUB_MACHINE1_DEBUG \
         -DSUB_MACHINE2_DEBUG \
         -DNEW_MACHINE_DEBUG \
         -I../ \
	 -Wall \
	 -ggdb

override FSM_FLAGS += --generate-run-function=true

include ../variants.mk

