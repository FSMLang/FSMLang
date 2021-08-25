##########################################
#
# makefile for individual test
#
#

VPATH=../../src

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
include $(OUTPUT_DIR)/source.mk
endif

override CFLAGS += -DPARSER_DEBUG -D$(ARCH) -I../ -I$(VPATH) -I .

TESTS=parser-test1  \
			parser-test2  \
			parser-test3  \
			parser-test4  \
			parser-test5  \
			parser-test6  \
			parser-test7  \
			parser-test8  \
			parser-test9  \
			parser-test10 \
			parser-test11

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS)

$(TESTS): test
	@./test $@.fsm > $@.out 2> $@.stderr
	@cat $@.stderr >> $@.out; rm $@.stderr
	@$(DIFF) $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm $@.out $@.result
      
%fsm_utils.o %lexer.o: y.tab.h

y.tab.h: parser.o

clean::
	-@rm *.[hc] > /dev/null

