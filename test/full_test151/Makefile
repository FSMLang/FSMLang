##########################################
#
# makefile for individual test
#
#

override CFLAGS += -I../ \
	 -Wall \
	 -ggdb \
    -DCOMMUNICATOR_DEBUG \
    -DCOMMUNICATOR_COMMANDS_DEBUG


override DIFF_FLAGS= -I \"warning: (test_fsm_sub_subSub): all events are handled identically in all states.\"  \
            -I \"warning: (test_fsm_sub2_subSub): all events are handled identically in all states.\"  

VARIANTS=c cc s e ss

#include ../weak_variants.mk
include ../variants.mk

FSM_FLAGS += -i0

cc_run: FSM_FLAGS += --force-generation-of-event-passing-actions

