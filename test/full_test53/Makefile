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

override FSM_PLANTUML_FLAGS += --add-plantuml-prefix-file=puml_prefix_string1 --add-plantuml-prefix-file=puml_prefix_string2

SRC = $(shell find . -name "*-actions.c" -printf "%P ")
FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

GENERATED_PLANTUML=subMachine1.plantuml \
                   subMachine2.plantuml

TARGET = check_top_level_plantuml

include ../test.mk
include ../../fsmrules.mk

check_top_level_plantuml: top_level.plantuml $(GENERATED_PLANTUML)
	@$(DIFF) top_level.plantuml top_level.plantuml.canonical     > test.result
	@$(DIFF) subMachine1.plantuml subMachine1.plantuml.canonical > test.result
	@$(DIFF) subMachine2.plantuml subMachine2.plantuml.canonical > test.result
	@rm -f test.result
	@rm -f *.plantuml

$(GENERATED_PLANTUML): top_level.plantuml
