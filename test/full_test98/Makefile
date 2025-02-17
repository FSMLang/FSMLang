##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DCOMPACT_DEBUG                   \
			-DCOMPACT_SUB_COMPACT_DEBUG                 \
			-DCOMPACT_SUB_COMPACT_SUB_SUB_COMPACT_DEBUG \
			-I../                                       \
	 		-Wall                                       \
	 		-ggdb

override FSM_FLAGS+=--empty-cell-fn=i_am_empty

include ../variants.mk


