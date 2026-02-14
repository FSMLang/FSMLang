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

VARIANTS=ss s c cc e

include ../variants.mk

.c.e:
	$(CC) -E $(CFLAGS) -o $@ $<
