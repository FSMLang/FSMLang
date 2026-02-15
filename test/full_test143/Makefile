##########################################
#
# makefile for individual test
#
#

.SUFFIXES: .e

override CFLAGS += -I../           \
	 -Wall                          \
	 -ggdb                          \
    -DTEST_FSM_DEBUG               \
    -DINIT_FSM_DATA0={.e1_count=0} \
    -DINIT_FSM_DATA1={.e1_count=1} \
    -DINIT_FSM_DATA2={.e1_count=2} 

override FSM_FLAGS+=-i3

DIFF_FLAGS=-I \"warning: (test_fsm): all events are handled identically in all states\\.\"

VARIANTS=ss s c cc e

include ../variants.mk

.c.e:
	$(CC) -E $(CFLAGS) -o $@ $<
