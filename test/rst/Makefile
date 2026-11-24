#
# manages rst tests
#
#
 
SUITES += $(shell find . -maxdepth 1 -type d -not -name "_*" -printf "%P ")

CLEAN_SUITES=$(addsuffix .clean, $(SUITES))

DOCS=$(addsuffix .doc, $(SUITES))

.PHONY: all clean $(SUITES) $(CLEAN_SUITES)

runtest: $(SUITES)
	@echo "all $(notdir $(shell pwd)) tests pass"

recordtest: runtest
	@echo $(notdir $(shell pwd)) >> ../done

clean: $(CLEAN_SUITES) cleandocs

$(SUITES):
	$(MAKE) -C $@ OUTPUT_DIR="$(OUTPUT_DIR)" runtest

$(CLEAN_SUITES):
	$(MAKE) -C $(basename $@) OUTPUT_DIR="$(OUTPUT_DIR)" clean

$(DOCS):
	@$(MAKE) -C $(basename $@) OUTPUT_DIR="$(OUTPUT_DIR)" SUITE="$(basename $@)" docsrc
	@set -e; sed -e 's/{{PROJECT}}/$(basename $@)/' < conf.py.tmpl > conf.py
	@$(MAKE) -f sphinx.mk html
	@rm -rf index.rst conf.py > /dev/null
	@$(MAKE) -C $(basename $@) OUTPUT_DIR="$(OUTPUT_DIR)" clean

cleandocs:
	@$(MAKE) -f sphinx.mk clean
	@rm -rf index.rst conf.py > /dev/null

check:
	@echo $(SUITES)

