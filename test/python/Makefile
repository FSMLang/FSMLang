#
# manages plantuml tests
#
#
 
SUITES += $(shell find . -maxdepth 1 -type d -printf "%P ")

CLEAN_SUITES=$(addsuffix .clean, $(SUITES))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

runtest: $(SUITES)

clean: $(CLEAN_SUITES)

$(SUITES):
	$(MAKE) -C $@ OUTPUT_DIR="$(OUTPUT_DIR)" runtest

$(CLEAN_SUITES):
	$(MAKE) -C $(basename $@) OUTPUT_DIR="$(OUTPUT_DIR)" clean

check:
	@echo $(SUITES)
