TESTS=$(shell find . -type d -printf "%P ")

CLEAN_TESTS = $(addsuffix .clean ,$(TESTS))
RUN_TESTS   = $(addsuffix .run   ,$(TESTS))

.PHONY: runtest all clean $(TESTS) $(CLEAN_TESTS)

override CFLAGS+=-DUNIT_TEST

all: runtest

test:
	@echo $(notdir $(CURDIR)): to run an individual test, move to this directory.

$(TESTS):
	$(MAKE) -C $@ CFLAGS="$(CFLAGS)" ARCH="$(ARCH)" test

$(CLEAN_TESTS):
	$(MAKE) -C $(basename $@) CFLAGS="$(CFLAGS)" clean

$(RUN_TESTS):
	$(MAKE) -C $(basename $@) CFLAGS="$(CFLAGS)" ARCH="$(ARCH)"  

clean: $(CLEAN_TESTS)

runtest: clean $(RUN_TESTS)
	@echo $(notdir $(PWD)) pass

