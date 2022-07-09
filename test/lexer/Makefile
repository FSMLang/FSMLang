##########################################
#
# makefile for individual test
#
#

VPATH=../../src

override CFLAGS += -DLEX_DEBUG -I../ -I$(VPATH)

SRC = lexer.c       \
			lexer_debug.c \
			list.c \
			fsm_utils.c

TESTS=lexer-test1 \
			lexer-test2 \
			lexer-test3 \
			lexer-test4 \
			lexer-test5 \
			lexer-test6 \
			lexer-test7 \
			lexer-test8

LDFLAGS=-lfl

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS)

$(TESTS): test
	./test < $@.txt > $@.out 2> $@.stderr
	cat $@.stderr >> $@.out; rm $@.stderr
	$(DIFF) $@.out $@.canonical > $@.result
	echo $@ successful
	rm $@.out $@.result

