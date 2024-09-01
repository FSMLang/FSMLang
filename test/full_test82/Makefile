##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DMESSENGER_DEBUG -I../
override CFLAGS += -DFSM_DEBUG -DTEST_FSM_DEBUG -I../

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

include ../variants.mk

