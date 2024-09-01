.PHONY: runtest test clean clean_generated

VARIANTS ?= c s e

e.size e_run: FSM_FLAGS=-te

c.size c_run: FSM_FLAGS=-tc

s.size s_run: FSM_FLAGS=-ts

runtest: $(addsuffix _run, $(VARIANTS))
	@echo "all tests successful"

test: sizes

sizes: $(addsuffix .size, $(VARIANTS))
	cat *.size >> sizes
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean

$(addsuffix .size, $(VARIANTS)):
	@echo $(FSM_FLAGS)
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" CFLAGS="$(CFLAGS)" test
	@echo -n "$@ size: " > $@
	@ls -l test | cut -f5 -d' ' >>  $@
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean

$(addsuffix _run, $(VARIANTS)):
	@echo $(FSM_FLAGS)
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" CFLAGS="$(CFLAGS)" do_runtest

clean:
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean
	-@rm $(addsuffix .size, $(VARIANTS))
	-@rm sizes

clean_generated:
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean

