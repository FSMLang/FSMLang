##########################################
#
# makefile for individual test
#
#

VPATH=../../src

override CFLAGS += -DLEX_DEBUG -I../ -I$(VPATH)

SRC = lexer.c       \
			lexer_debug.c \
			fsm_utils.c

TESTS=lexer-test1 \
			lexer-test2 \
			lexer-test3 \
			lexer-test4 \
			lexer-test5 \
			lexer-test6 \
			lexer-test7 \
			lexer-test8 \
			lexer-test9 \
			lexer-test10

LDFLAGS=-lfl

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS)

$(TESTS): test
	./test < $@.txt &> $@.out
	@diff $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm $@.out $@.result

