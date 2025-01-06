##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
         -DSUB_DEBUG \
         -DSUB_SUB_DEBUG \
         -DTOP_LEVEL_DEBUG \
         -I../ \
	 -Wall \
	 -ggdb

override FSM_FLAGS=--include-svg-img

include ../variants.mk


