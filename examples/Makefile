# Build and check all of the examples

DIRS=$(wildcard */)
EXAMPLES=$(patsubst %/, %, $(DIRS))
CLEAN_EXAMPLES=$(addsuffix .clean, $(EXAMPLES))
DONE = $(shell cat done)
TODO = $(filter-out $(DONE), $(EXAMPLES))

.PHONY: $(EXAMPLES) $(CLEAN_EXAMPLES) clean echo_examples build_examples

build_examples: $(EXAMPLES)
	@$(MAKE) clean

$(EXAMPLES):
	@$(MAKE) -C $@ check
	@echo -n "$@ " >> done

clean: $(CLEAN_EXAMPLES)
	-@rm -f done      2> /dev/null

$(CLEAN_EXAMPLES):
	-@$(MAKE) -C $(basename $@) clean

echo_examples:
	@echo $(EXAMPLES)

