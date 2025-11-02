##########################################
#
# makefile for individual test
#
#

.SUFFIXES: .e

override CFLAGS += -I../ \
	 -Wall \
	 -ggdb

override FSM_FLAGS+=-i0
VARIANTS=c s e

include ../variants.mk

.c.e:
	$(CC) -E $(CFLAGS) -o $@ $<
