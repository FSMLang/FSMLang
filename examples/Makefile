# Build and check all of the examples

DIRS=$(wildcard */)
EXAMPLES=$(patsubst %/, %, $(DIRS))
CLEAN_EXAMPLES=$(addsuffix .clean, $(EXAMPLES))
DONE = $(shell cat done)
TODO = $(filter-out $(DONE), $(EXAMPLES))

.PHONY: $(EXAMPLES)

$(EXAMPLES):
	@$(MAKE) -C $@ check
	@echo -n "$@ " >> done

clean: $(CLEAN_EXAMPLES)
	-@rm done      2> /dev/null

$(CLEAN_EXAMPLES):
	$(MAKE) -C $(basename $@) clean
