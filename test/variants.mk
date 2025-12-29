.SECONDEXPANSION:

VARIANTS ?= c s e cc sc

.PHONY: runtest test clean clean_generated $(VARIANTS) check_fsmout

.SUFFIXES: .e

include $(OUTPUT_DIR)/system.mk

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
cc cc.size cc_run: DIFF_FLAGS+=-I \"Warning: Ignoring compact array request because actions do not return appropriately\\.\"

sc sc.size sc_run: FSM_FLAGS+=-ts -c --generate-weak-fns=false  --force-generation-of-event-passing-actions
sc sc.size sc_run: CFLAGS+=-DFSM_VARIANT_SC
sc sc.size sc_run: DIFF_FLAGS+=-I \"Warning: Ignoring external event designations\"
sc sc.size sc_run: DIFF_FLAGS+=-I \"Warning: Ignoring compact array request because actions do not return appropriately\\.\"

TARGET ?= do_runtest

runtest: $(TARGET)
	@echo "all tests successful"

do_runtest: $(addsuffix _run, $(VARIANTS))

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
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" CFLAGS="$(CFLAGS)" DIFF_FLAGS="$(DIFF_FLAGS)" runtest

clean:
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean
	-@rm -f sizes $(VARIANTS) $(addsuffix .size, $(VARIANTS)) 2> /dev/null
	-@rm -f *.e 2> /dev/null

clean_generated:
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean

$(VARIANTS):
	$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" DIFF_FLAGS="$(DIFF_FLAGS)" VARIANTS="$@" $@
	@cp fsmout $@

$(addsuffix .fsmout, $(VARIANTS)): $$(basename $$@)
	@echo Checking: $(basename $<)
	@$(DIFF) $(basename $<) $(basename $<).canonical
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean

stats.txt:
	$(MAKE) -f ../create_target.mk $@

check_fsmout: clean $(addsuffix .fsmout, $(VARIANTS))
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean
	@rm -rf $(VARIANTS) 2> /dev/null

.c.e:
	$(CC) -E $(CFLAGS) -o $@ $<
