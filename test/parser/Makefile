##########################################
#
# makefile for individual test
#
#

VPATH=../../src

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
#include $(OUTPUT_DIR)/source.mk
endif

SRC =	lexer.c        \
			parser.c    \
			fsm_utils.c \
			list.c      \
			ancestry.c  \
			cwalk.c     \
			usage.c

ifdef NEED_RINDEX
SRC += rindex.c
endif

override CFLAGS += -ggdb -DPARSER_DEBUG -D$(ARCH) -I../ -I$(VPATH) -I .

FSMS = $(shell find . -name "*.fsm" -printf "%P ")
TESTS = $(basename $(FSMS))

echotests:
	@echo $(TESTS)

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS) 
	@echo "all parser tests pass"

$(TESTS): test 
	@./test $@.fsm > $@.out 2> $@.stderr
	@cat $@.stderr >> $@.out; rm -f $@.stderr
	@$(DIFF) $@.out $@.canonical
	@echo $@ successful
	@rm -f $@.out $@.result
      
#
# This is not working on GitHub, though it does locally.
#
test_multiple_files: test
	echo $(DIFF)
	./test $(FSMS) > $@.out 2> $@.stderr
	@cat $@.stderr >> $@.out; rm -f $@.stderr
	@$(DIFF) $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm -f $@.out $@.result

%fsm_utils.o %lexer.o: y.tab.h

y.tab.h: parser.o

clean::
	-@rm -f *.[hc] > /dev/null

