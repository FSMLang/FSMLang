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

FSM_PLANTUML_FLAGS = --add-plantuml-title --add-plantuml-legend=top

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_SRC=sub1.c

TARGET     = check_plantuml
TEST_INPUT = $(GENERATED_SRC:.c=.plantuml) $(FSM_SRC:.fsm=.plantuml)

include ../test.mk
include ../../fsmrules.mk

check_plantuml: $(TEST_INPUT)
	$(DIFF) comment_test.plantuml comment_test.plantuml.canonical > test.result
	$(DIFF) sub1.plantuml sub1.plantuml.canonical > test.result
	-@rm -f $(TEST_INPUT)
	-@rm -f test.result
	-@rm -f fsmout

$(GENERATED_SRC:.c=.plantuml) : $(FSM_SRC:.fsm=.plantuml)


