##########################################
#
# makefile for individual test
#
#

FSM_PLANTUML_FLAGS = --add-plantuml-title --add-plantuml-legend="top,left"

TARGET     = check_plantuml

include ../test.mk
include ../../fsmrules.mk

check_plantuml: $(GENERATED_PLANTUML)
	$(DIFF) comment_test.plantuml comment_test.plantuml.canonical > test.result
	$(DIFF) sub1.plantuml sub1.plantuml.canonical > test.result
	-@rm -f $(GENERATED_PLANTUML)
	-@rm -f test.result
	-@rm -f fsmout


