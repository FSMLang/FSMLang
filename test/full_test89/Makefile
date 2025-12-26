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

override FSM_FLAGS+=--add-profiling-macros=true

DIFF_FLAGS= -I \"warning: (test_sub): all events are handled identically in all states\\.\"         

SRC=profiling.c

include ../variants.mk


