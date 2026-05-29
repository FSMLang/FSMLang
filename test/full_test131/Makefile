##########################################
#
# makefile for individual test
#
#

.PHONY: clean

override CFLAGS += -I../ \
	 -Wall \
	 -ggdb \
    -DTEST_FSM_DEBUG


TARGET=fsm_fail_is_pass

ifneq ($(MAKECMDGOALS),clean)
include ../../fsmrules.mk
include ../test.mk
else
clean:
endif

