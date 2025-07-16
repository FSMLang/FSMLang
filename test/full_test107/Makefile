.PHONEY: check_html

FSM_HTML_FLAGS=-th --css-content-filename=$(OUTPUT_DIR)/../fsmlang.css --css-content-internal=true --include-svg-img

TARGET = check_html

PLANTUML=plantuml

FSM_PLANTUML_FLAGS=-tp --add-plantuml-title

include ../../fsmrules.mk

include ../test.mk

check_html: $(GENERATED_HTML)
	echo $<
	set -e; \
	for f in $(GENERATED_HTML);\
		do $(DIFF)  $$f $$f.canonical > test.result;\
	done
	-@rm -f $(GENERATED_HTML)
	-@rm -f test.result
	-@rm -f fsmout

clean::
	-@rm -f $(GENERATED_HTML)     2> /dev/null
	-@rm -f $(GENERATED_PLANTUML) 2> /dev/null
	-@rm -f *.svg                 2> /dev/null

# SVG images are handy
svg: $(GENERATED_PLANTUML)
	@$(PLANTUML) -tsvg *.plantuml

