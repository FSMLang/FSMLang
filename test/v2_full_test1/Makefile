##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
         -DSUB_DEBUG \
         -DTEST_FSM_DEBUG \
         -I../ \
	 -Wall \
	 -ggdb

override FSM_FLAGS+=--add-profiling-macros=true --profile-sub-fsms=true

SRC=profiling.c

include ../variants.mk


