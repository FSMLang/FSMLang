##########################################
#
# makefile for individual test
#
#

override CFLAGS += -I../ \
	 -Wall \
	 -ggdb

DIFF_FLAGS= -I \"warning: (test_fsm): all events are handled identically in all states\\.\"             \
            -I \"warning: (test_fsm_sub): all events are handled identically in all states\\.\"         \
            -I \"warning: (test_fsm_sub_basement): all events are handled identically in all states\\.\"  

VARIANTS=c s e

include ../variants.mk


