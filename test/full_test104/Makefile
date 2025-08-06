.PHONEY: check_plantuml

PLANTUML=plantuml

FSM_PLANTUML_FLAGS=-tp --add-plantuml-title

TARGET = check_plantuml

include ../../fsmrules.mk

include ../test.mk

check_plantuml: $(GENERATED_PLANTUML)
	echo $<
	set -e; \
	for f in $(GENERATED_PLANTUML);\
		do $(DIFF)  $$f $$f.canonical > $$f.result;\
	done
	-@rm -f $(GENERATED_PLANTUML)
	-@rm -f test.result
	-@rm -f fsmout

# SVG images are handy
svg: $(GENERATED_PLANTUML)
	@$(PLANTUML) -tsvg *.plantuml

clean::
	-@rm -f *.svg 2> /dev/null

