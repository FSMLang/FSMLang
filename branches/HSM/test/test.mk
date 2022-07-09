#######################################
#
# Generic rules for creating and running tests.
#
#

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
endif

include ../../depends.mk

OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o)

ifndef NO_RUNTEST
runtest: test
	@./test > test.out 2> test.stderr
	@cat test.stderr >> test.out
	@cat fsmout >> test.out
	@$(DIFF) test.out test.canonical > test.result
	@echo "Test successful"
	@rm test.out test.result test.stderr
endif

test: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean::
	-@rm *.exe 2> /dev/null
	-@rm *.out 2> /dev/null
	-@rm *.result 2> /dev/null
	-@rm *.stderr 2> /dev/null
	-@rm fsmout 2> /dev/null
	-@rm *.d* 2> /dev/null
	-@rm *.o 2> /dev/null
	-@rm *.stackdump 2> /dev/null
	-@rm $(FSM_SRC:.fsm=.c) 2> /dev/null
	-@rm $(FSM_SRC:.fsm=.h) 2> /dev/null
	-@rm test

