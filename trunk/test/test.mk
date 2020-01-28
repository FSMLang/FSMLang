#######################################
#
# Generic rules for creating an running tests.
#
#

include ../../depends.mk

OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o)

ifndef NO_RUNTEST
runtest: test
	@./test > test.out
	@diff test.out test.canonical > test.result
	@echo "Test successful"
	@rm test.out test.result
endif

test: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean::
	-@rm *.exe 2> /dev/nul
	-@rm *.out 2> /dev/nul
	-@rm *.result 2> /dev/nul
	-@rm *.d* 2> /dev/nul
	-@rm *.o 2> /dev/nul
	-@rm *.stackdump 2> /dev/nul
	-@rm $(FSM_SRC:.fsm=.c) 2> /dev/nul
	-@rm $(FSM_SRC:.fsm=.h) 2> /dev/nul

