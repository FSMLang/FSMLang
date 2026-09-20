##########################################
#
# makefile for individual test
#
#

.PHONY: docs

override CFLAGS += -I../                  \
	 -Wall                                 \
	 -ggdb                                 \
    -DTEST_FSM_DEBUG                      \
    -DTEST_FSM_E1_TRANSLATOR_DEBUG


override DIFF_FLAGS= -I \"warning: (test_fsm_sub_subSub): all events are handled identically in all states.\"  \
            -I \"warning: (test_fsm_sub2_subSub): all events are handled identically in all states.\"  

VARIANTS=s
#TARGET=pass

#include ../weak_variants.mk
include ../variants.mk


include ../../fsmrules.mk

pass:

