##########################################
#
# makefile for individual test
#
#

.SUFFIXES: .e

override CFLAGS += -I../ \
	 -Wall \
	 -ggdb \
    -DTEST_FSM_DEBUG

override FSM_FLAGS+=-i0

DIFF_FLAGS=-I \"warning: (test_fsm): all events are handled identically in all states\\.\"

VARIANTS=ss s c cc e

include ../variants.mk

.c.e:
	$(CC) -E $(CFLAGS) -o $@ $<
