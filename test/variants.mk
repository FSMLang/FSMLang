VARIANTS ?= c s e

.PHONY: runtest test clean clean_generated $(VARIANTS)

#e e.size e_run: FSM_FLAGS=-te
e e.size e_run: FSM_FLAGS+=-te --generate-weak-fns=false --force-generation-of-event-passing-actions

c c.size c_run: FSM_FLAGS+=-tc --generate-weak-fns=false --force-generation-of-event-passing-actions

#s s.size s_run: FSM_FLAGS=-ts
s c.size s_run: FSM_FLAGS+=-ts --generate-weak-fns=false --force-generation-of-event-passing-actions

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
	-@rm -f sizes $(VARIANTS) $(addsuffix .size, $(VARIANTS)) 2> /dev/null

clean_generated:
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean

$(VARIANTS):
	$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" VARIANTS="$(VARIANTS)" $@

stats.txt:
	$(MAKE) -f ../create_target.mk $@

