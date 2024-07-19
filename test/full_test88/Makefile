##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
         -DSUB_MACHINE1_DEBUG \
         -DSUB_MACHINE2_DEBUG \
         -DNEW_MACHINE_DEBUG \
         -I../ \
	 -Wall \
	 -ggdb

override FSM_FLAGS += --convenience-macros-in-public-header=false --generate-run-function=true -ts

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

TARGET=clean_fail_test_c

include ../../fsmrules.mk

include ../test.mk

clean_fail_test_c: fail_test_c clean

fail_test_c: top_level.h
	@$(CC) -o $(CFLAGS) top_level.c $(CALL_FAILURE_A_SUCCESS)

