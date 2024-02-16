##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG -DNEW_MACHINE_DEBUG -I../

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

TARGET=fsm_fail_is_pass

include ../test.mk
include ../../fsmrules.mk
