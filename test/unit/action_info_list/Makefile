.SECONDEXPANSION:

override CFLAGS += -Wall -Wpedantic -Wextra -DUNIT_TEST
override CFLAGS += -I../ -I$(VPATH) -DARCH=$(ARCH)


VPATH=../../../src

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
VPATH=$(OUTPUT_DIR)/../src
endif

SRC=action_info_list.c list.c
OBJS=$(SRC:.c=.o)

TESTS=$(basename $(shell find -L . -type f -name "test_*.c" -printf "%P "))

LIB_SRC=grist.c print_consolidated.c
LIB_OBJ=$(LIB_SRC:.c=.o) $(SRC:.c=.o)

libu.a: $(LIB_OBJ)
	@$(AR) rcs $@ $(LIB_OBJ)

$(TESTS): $$(addsuffix .o, $$@) libu.a
	gcc -o $@ $@.o -L. -lu

$(addsuffix .run, $(TESTS)): $$(basename $$@)
	./$(basename $@) > $(basename $@).out
	$(DIFF) $(basename $@).out $(basename $@).canonical > $(basename $@).result
	@echo $(basename $@) passes.

runtest: $(addsuffix .run, $(TESTS)) clean
	@echo All tests pass.

check_macros:
	@echo TESTS=$(TESTS)

clean:
	-@rm -rf *.a *.o 2> /dev/null
	-@rm -rf $(TESTS) 2> /dev/null
	-@rm -rf *.result 2> /dev/null


