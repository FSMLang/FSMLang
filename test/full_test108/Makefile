.PHONEY: check_rst sphinx

TARGET = check_rst

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
	-@rm -f *.svg                         2> /dev/null
	-@rm -f sphinx/*.svg                  2> /dev/null
	-@$(MAKE) -C sphinx clean

sphinx: $(GENERATED_RST)
	mv $(GENERATED_RST) sphinx/
	$(MAKE) -C sphinx html

