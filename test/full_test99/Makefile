##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DTEST_EMPTY_CELLS_DEBUG \
			-I../                              \
	 		-Wall                              \
	 		-ggdb

override FSM_FLAGS+=--empty-cell-fn=i_am_empty

include ../variants.mk


