#######################################
#
# Generic rules for creating and running tests.
#
#

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
endif

include ../../depends.mk

OBJS = $(SRC:.c=.o) $(FSM_SRC:.fsm=.o) $(GENERATED_SRC:.c=.o)

TARGET ?= do_runtest

ifndef NO_RUNTEST
runtest: $(TARGET)
	@echo "Test successful"
endif

do_runtest: test
	@./test > test.out 2> test.stderr
	@cat test.stderr >> test.out
	@cat fsmout >> test.out
	@$(DIFF) test.out test.canonical > test.result
	@rm test.out test.result test.stderr

test: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

fail_is_pass: $(OBJS)
	@$(CC) -o $@ $(OBJS) $(LDFLAGS); \
	if [ $$? -ne 0 ]; \
	then \
		echo "expected failure; test passes"; \
		true; \
	fi

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
	-@rm $(FSM_SRC:.fsm=.html) 2> /dev/null
	-@rm $(FSM_SRC:.fsm=.svg) 2> /dev/null
	-@rm $(FSM_SRC:.fsm=.plantuml) 2> /dev/null
	-@rm $(GENERATED_SRC) 2> /dev/null
	-@rm $(GENERATED_SRC:.c=.h) 2> /dev/null
	-@rm $(GENERATED_SRC:.c=.html) 2> /dev/null
	-@rm $(GENERATED_SRC:.c=.html) 2> /dev/null
	-@rm $(GENERATED_SRC:.c=.svg) 2> /dev/null
	-@rm $(GENERATED_SRC:.c=.plantuml) 2> /dev/null
	-@rm test

