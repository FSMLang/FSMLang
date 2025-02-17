##########################################
#
# makefile for individual test
#
#

FSM_PLANTUML_FLAGS = --add-plantuml-title --add-plantuml-legend=right

TARGET     = check_plantuml

include ../test.mk
include ../../fsmrules.mk

check_plantuml: $(GENERATED_PLANTUML)
	$(DIFF) comment_test.plantuml comment_test.plantuml.canonical > test.result
	$(DIFF) sub1.plantuml sub1.plantuml.canonical > test.result
	-@rm -f test.result   2> /dev/null
	-@rm -f fsmout        2> /dev/null

