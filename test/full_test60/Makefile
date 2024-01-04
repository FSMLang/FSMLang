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

FSM_PLANTUML_FLAGS = --add-plantuml-title --add-plantuml-legend=left

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

TARGET     = check_plantuml
TEST_INPUT = $(shell $(FSM) -M -tp $(FSM_SRC))

include ../test.mk
include ../../fsmrules.mk

check_plantuml: $(TEST_INPUT)
	$(DIFF) comment_test.plantuml comment_test.plantuml.canonical > test.result
	$(DIFF) sub1.plantuml sub1.plantuml.canonical > test.result
	-@rm $(TEST_INPUT)
	-@rm test.result
	-@rm fsmout


