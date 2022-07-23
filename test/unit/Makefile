TESTS=$(shell find . -type d -printf "%P ")

CLEAN_TESTS = $(addsuffix .clean ,$(TESTS))
RUN_TESTS   = $(addsuffix .run   ,$(TESTS))

.PHONY: runtest all clean $(TESTS) $(CLEAN_TESTS)

all: runtest

$(TESTS):
	$(MAKE) -C $@ CFLAGS="$(CFLAGS)" test

$(CLEAN_TESTS):
	$(MAKE) -C $(basename $@) CFLAGS="$(CFLAGS)" clean

$(RUN_TESTS):
	$(MAKE) -C $(basename $@) CFLAGS="$(CFLAGS)" runtest

clean: $(CLEAN_TESTS)

runtest: clean $(RUN_TESTS)
	@echo "Unit tests pass"
