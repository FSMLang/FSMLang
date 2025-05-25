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

FSM_FLAGS = --inhibiting-states-share-events=true

include ../variants.mk

