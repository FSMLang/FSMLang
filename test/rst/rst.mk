
.PHONY: $(TESTS) docsrc echotests ../index.rst
.SECONDEXPANSION:

FSMS = $(shell find . -name "*.fsm" -printf "%P ")
TESTS = $(basename $(FSMS))

NO_RUNTEST=TRUE
include $(OUTPUT_DIR)/../test/test.mk

include $(OUTPUT_DIR)/../fsmrules.mk

echotests:
	@echo $(TESTS)

runtest: $(TESTS) 
	@echo "all rst tests pass"
	@$(MAKE) clean

../index.rst:
	@../../../rst_title.py $(SUITE) > ../index.rst
	@echo -n "\n\n.. toctree::\n\n" >> ../index.rst
	@set -e; \
	for f in $(GENERATED_RST); do \
		echo -n "\t$(SUITE)/$$f\n" >> ../index.rst; \
	done
	@echo -n "\n" >> ../index.rst
	@cat ../index.rst

docsrc: $(GENERATED_PLANTUML) $(GENERATED_RST) ../index.rst

$(TESTS): $(FSM) $(GENERATED_RST) Makefile
	@set -e; \
	for f in $(filter %.rst, $^); do \
		$(DIFF) $$f $$f.canonical > $${f%.*}.result; \
	done
	@echo $@ successful
	@rm -f $@.rst $@.result > /dev/null
      
clean::
	-@rm -f *.rst > /dev/null
	-@rm -f *.plantuml > /dev/null
	-@rm -f *.stderr > /dev/null
	-@rm -f *.result > /dev/null
	-@rm -f *.fsmrh  > /dev/null
	-@rm -f fsmout  > /dev/null



