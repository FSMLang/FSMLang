.PHONEY: check_plantuml

FSM_PLANTUML_FLAGS=-tp --add-plantuml-title

TARGET = check_plantuml

include ../../fsmrules.mk

include ../test.mk

check_plantuml: $(GENERATED_PLANTUML)
	echo $<
	set -e; \
	for f in $(GENERATED_PLANTUML);\
		do $(DIFF)  $$f $$f.canonical > test.result;\
	done
	-@rm -f $(GENERATED_PLANTUML)
	-@rm -f test.result
	-@rm -f fsmout

