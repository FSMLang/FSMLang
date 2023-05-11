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

SRC =	lexer.c \
			parser.c \
			fsm_utils.c \
			list.c \
			cwalk.c

ifdef NEED_RINDEX
SRC += rindex.c
endif

override CFLAGS += -ggdb -DPARSER_DEBUG -D$(ARCH) -I../ -I$(VPATH) -I .

TESTS = $(basename $(shell find . -name "*.fsm" -printf "%P "))

echotests:
	@echo $(TESTS)

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS)
	@echo "all parser tests pass"

$(TESTS): test 
	./test $@.fsm > $@.out 2> $@.stderr
	cat $@.stderr >> $@.out; rm $@.stderr
	@$(DIFF) $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm $@.out $@.result
      
%fsm_utils.o %lexer.o: y.tab.h

y.tab.h: parser.o

clean::
	-@rm *.[hc] > /dev/null

