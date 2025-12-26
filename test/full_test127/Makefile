##########################################
#
# makefile for individual test
#
#

override CFLAGS += -I../ \
	 -Wall \
	 -ggdb

override FSM_FLAGS += -i3

DIFF_FLAGS= -I \"warning: (test_fsm): all events are handled identically in all states\\.\"             \
            -I \"warning: (test_fsm_sub): all events are handled identically in all states\\.\"         \
            -I \"warning: (test_fsm_sub_subSub): all events are handled identically in all states\\.\"  \
            -I \"warning: (test_fsm_sub2_subSub): all events are handled identically in all states\\.\" \
            -I \"warning: (test_fsm_sub2): all events are handled identically in all states\\.\"

VARIANTS=c s e

include ../variants.mk


