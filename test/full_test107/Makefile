.PHONEY: check_rst sphinx

FSM_RST_FLAGS=--include-svg-img

TARGET = check_rst

PLANTUML=plantuml

FSM_PLANTUML_FLAGS=-tp --add-plantuml-title

include ../../fsmrules.mk

include ../test.mk

echo:
	@echo $(GENERATED_RST)


check_rst: $(GENERATED_RST)
	echo $<
	set -e; \
	for f in $(GENERATED_RST);\
		do $(DIFF)  $$f $$f.canonical > test.result;\
	done
	-@rm -f $(GENERATED_RST)
	-@rm -f test.result
	-@rm -f fsmout

clean::
	-@rm -f $(GENERATED_RST)              2> /dev/null
	-@cd sphinx && rm -f $(GENERATED_RST) 2> /dev/null
	-@rm -f $(GENERATED_PLANTUML)         2> /dev/null
	-@rm -f *.svg                         2> /dev/null
	-@rm -f sphinx/*.svg                  2> /dev/null
	-@$(MAKE) -C sphinx clean

# SVG images are handy
svg: $(GENERATED_PLANTUML)
	@$(PLANTUML) -tsvg *.plantuml

sphinx: $(GENERATED_RST) svg
	mv $(GENERATED_RST) sphinx/
	mv *.svg sphinx
	$(MAKE) -C sphinx html

