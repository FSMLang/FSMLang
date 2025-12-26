##########################################
#
# makefile for individual test
#
#

override CFLAGS += -I../ \
	 -Wall \
	 -ggdb

override DIFF_FLAGS=-I \"warning: (test_fsm): all events are handled identically in all states\\.\"

VARIANTS=c s e

include ../variants.mk


