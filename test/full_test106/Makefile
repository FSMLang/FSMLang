.PHONEY: check_html

FSM_HTML_FLAGS=-th --css-content-filename=$(OUTPUT_DIR)/../fsmlang.css --css-content-internal=true

TARGET = check_html

include ../../fsmrules.mk

include ../test.mk

check_html: $(GENERATED_HTML)
	echo $<
	set -e; \
	for f in $(GENERATED_HTML);\
		do $(DIFF)  $$f $$f.canonical > $$f.result;\
	done
	-@rm -f $(GENERATED_HTML)
	-@rm -f test.result
	-@rm -f fsmout

clean::
	-@rm -f $(GENERATED_HTML) 2> /dev/null

