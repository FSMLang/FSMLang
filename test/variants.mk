VARIANTS ?= c s e cc

.PHONY: runtest test clean clean_generated $(VARIANTS)

.SUFFIXES: .e

#e e.size e_run: FSM_FLAGS=-te
e e.size e_run: FSM_FLAGS+=-te --generate-weak-fns=false --force-generation-of-event-passing-actions
e e.size e_run: CFLAGS+=-DFSM_VARIANT_E

c c.size c_run: FSM_FLAGS+=-tc --generate-weak-fns=false --force-generation-of-event-passing-actions
c c.size c_run: CFLAGS+=-DFSM_VARIANT_C

#s s.size s_run: FSM_FLAGS=-ts
s s.size s_run: FSM_FLAGS+=-ts --generate-weak-fns=false --force-generation-of-event-passing-actions
s s.size s_run: CFLAGS+=-DFSM_VARIANT_S

cc cc.size cc_run: FSM_FLAGS+=-tc -c --generate-weak-fns=false
cc cc.size cc_run: CFLAGS+=-DFSM_VARIANT_CC

sc sc.size sc_run: FSM_FLAGS+=-ts -c --generate-weak-fns=false --force-generation-of-event-passing-actions
sc sc.size sc_run: CFLAGS+=-DFSM_VARIANT_SC

runtest: $(addsuffix _run, $(VARIANTS))
	@echo "all tests successful"

recordtest: clean runtest
	@echo $(notdir $(shell pwd)) >> ../done

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
	@echo $@: $(FSM_FLAGS)
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" CFLAGS="$(CFLAGS)" do_runtest

clean:
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean
	-@rm -f sizes $(VARIANTS) $(addsuffix .size, $(VARIANTS)) 2> /dev/null
	-@rm -f *.e 2> /dev/null

clean_generated:
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean

$(VARIANTS):
	$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" VARIANTS="$@" $@

stats.txt:
	$(MAKE) -f ../create_target.mk $@

.c.e:
	$(CC) -E $(CFLAGS) -o $@ $<
