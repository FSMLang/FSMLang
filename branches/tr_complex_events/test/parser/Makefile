##########################################
#
# makefile for individual test
#
#

VPATH=../../src

LEX = flex -I
YACC = bison -d -y
CC = gcc -g

override CFLAGS += -DPARSER_DEBUG -D$(ARCH) -I../ -I$(VPATH) -I .

SRC = lexer.c    \
			parser.c   \
			revision.c \
			fsm_html.c \
			fsm_c.c    \
			fsm_utils.c

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
			parser-test11 \
			parser-test12 \
			parser-test13 \
			parser-test14

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS)

$(TESTS): test
	./test $@.fsm &> $@.out
	@diff $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm $@.out $@.result
      
%fsm_utils.o %lexer.o: y.tab.h

y.tab.h: parser.o

clean::
	-@rm *.[hc] > /dev/nul

parser.o: parser.y

