##########################################
#
# makefile for individual test
#
#

.SUFFIXES: .e

override CFLAGS += -I../                                   \
	 -Wall                                                  \
	 -ggdb                                                  \
    -DTEST_FSM_DEBUG                                       \
    -DINIT_FSM_DATA0={.e1_count=0,.e2_count=0,.e3_count=0} \
    -DINIT_FSM_DATA1={.e1_count=1,.e2_count=1,.e3_count=1} \
    -DINIT_FSM_DATA2={.e1_count=2,.e2_count=2,.e3_count=2} 

override FSM_FLAGS+=-i3

VARIANTS=ss s c cc e

include ../variants.mk

.c.e:
	$(CC) -E $(CFLAGS) -o $@ $<
