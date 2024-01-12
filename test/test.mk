#######################################
#
# Generic rules for creating and running tests.
#
#
.PHONY: do_runtest show_objs

override CFLAGS += -Wall -Wpedantic -Wextra -I../


ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
endif

include ../../depends.mk

POSSIBLE_OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o) $(GENERATED_SRC:.c=.o)
OBJS = $(sort $(POSSIBLE_OBJS))

TARGET ?= do_runtest

CALL_FAILURE_A_SUCCESS = ; if [ $$? -ne 0 ]; then echo "expected failure; test passes"; true; else echo "did not find an expected failure; test fails"; false; fi


ifndef NO_RUNTEST
runtest: $(TARGET)
	@echo "Test successful"
endif

do_runtest: test
	@./test > test.out 2> test.stderr
	@cat test.stderr >> test.out
	@cat fsmout >> test.out
	@$(DIFF) test.out test.canonical > test.result
	@-rm -f test test.out test.result test.stderr
	@-rm -f fsmout                         2> /dev/null
	@-rm -f *.d*                           2> /dev/null
	@-rm -f *.fsmd*                        2> /dev/null
	@-rm -f *.o                            2> /dev/null
	@-rm -f $(GENERATED_SRC)               2> /dev/null
	@-rm -f $(GENERATED_HDR)               2> /dev/null
	@-rm -f $(GENERATED_PLANTUML)          2> /dev/null

test: $(OBJS)
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

fail_is_pass: $(OBJS)
	@$(CC) -o $@ $(OBJS) $(LDFLAGS) $(CALL_FAILURE_A_SUCCESS)

fsm_fail_is_pass: $(FSM_SRC:.fsm=.c)
	@echo "fsm failure expectation"

show_objs:
	@echo $(OBJS)

clean::
	-@rm -f *.exe                          2> /dev/null
	-@rm -f *.out                          2> /dev/null
	-@rm -f *.result                       2> /dev/null
	-@rm -f *.stderr                       2> /dev/null
	-@rm -f fsmout                         2> /dev/null
	-@rm -f *.d*                           2> /dev/null
	-@rm -f *.fsmd*                        2> /dev/null
	-@rm -f *.o                            2> /dev/null
	-@rm -f *.stackdump                    2> /dev/null
	-@rm -f $(GENERATED_SRC)               2> /dev/null
	-@rm -f $(GENERATED_HDR)               2> /dev/null
	-@rm -f $(GENERATED_PLANTUML)          2> /dev/null
	-@rm -f test                           2> /dev/null
	-@rm -f y.output                       2> /dev/null
	-@rm -f lexer.c                        2> /dev/null

