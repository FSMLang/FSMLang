##########################################
#
# makefile for individual test
#
#

override CFLAGS += -I../ \
	 -Wall \
	 -ggdb


override DIFF_FLAGS= -I \"warning: (test_fsm_sub_subSub): all events are handled identically in all states.\"  \
            -I \"warning: (test_fsm_sub2_subSub): all events are handled identically in all states.\" 

VARIANTS=sc

include ../variants.mk


