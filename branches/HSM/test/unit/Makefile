TESTS=$(shell find . -type d -printf "%P ")

CLEAN_TESTS = $(addsuffix .clean ,$(TESTS))
RUN_TESTS   = $(addsuffix .run   ,$(TESTS))

.PHONY: runtest all clean $(TESTS) $(CLEAN_TESTS)

all: runtest

$(TESTS):
	$(MAKE) -C $@ ARCH="$(ARCH)" -f ../unitTest.mk test

$(CLEAN_TESTS):
	$(MAKE) -C $(basename $@) -f ../unitTest.mk clean

$(RUN_TESTS):
	$(MAKE) -C $(basename $@) -f ../unitTest.mk ARCH="$(ARCH)" runtest

clean: $(CLEAN_TESTS)

runtest: clean $(RUN_TESTS)
	@echo "Unit tests pass"
