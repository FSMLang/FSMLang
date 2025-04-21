# Build and check all of the examples

DIRS=$(wildcard */)
EXAMPLES=$(patsubst %/, %, $(DIRS))

.PHONY: $(EXAMPLES)

$(EXAMPLES):
	@$(MAKE) -C $@ check


