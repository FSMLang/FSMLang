##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DMESSENGER_DEBUG -I../
override CFLAGS += -DFSM_DEBUG -DTEST_FSM_DEBUG -I../

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

VARIANTS = c s e

e e_run: FSM_FLAGS=-te

c c_run: FSM_FLAGS=-tc

s s_run: FSM_FLAGS=-ts

runtest: $(addsuffix _run, $(VARIANTS))
	@echo "all tests successful"

test: $(VARIANTS)
	cat *.size >> sizes

$(VARIANTS): 
	@echo $(FSM_FLAGS)
	@$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" test
	@echo -n "$@ size: " > $@.size
	@ls -l test | cut -f5 -d' ' >>  $@.size
	@$(MAKE) -f ../create_target.mk clean

$(addsuffix _run, $(VARIANTS)):
	@echo $(FSM_FLAGS)
	$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" do_runtest

clean:
	$(MAKE) -f ../create_target.mk FSM_FLAGS="$(FSM_FLAGS)" clean
	-@rm $(addsuffix .size, $(VARIANTS))
	-@rm sizes

