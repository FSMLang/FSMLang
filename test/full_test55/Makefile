##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
                   -DNEW_MACHINE_DEBUG \
                   -DSUB_MACHINE1_DEBUG \
                   -DSUB_MACHINE2_DEBUG \
                   -I../

override FSM_FLAGS += --generate-run-function

include ../variants.mk

