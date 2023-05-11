##########################################
#
# makefile for individual test
#
#

ifdef OUTPUT_DIR
include $(OUTPUT_DIR)/system.mk
#include $(OUTPUT_DIR)/source.mk
endif

VPATH=../../src

override CFLAGS += -DLEX_DEBUG -I../ -I$(VPATH)

SRC = lexer.c       \
			lexer_debug.c \
			list.c \
			fsm_utils.c

#TESTS=lexer-test1 \
#			lexer-test2 \
#			lexer-test3 \
#			lexer-test4 \
#			lexer-test5 \
#			lexer-test6 \
#			lexer-test7 \
#			lexer-test8 \
#			lexer-test9
#

TESTS = $(basename $(shell find . -name "lexer-*.txt" -printf "%P "))

LDFLAGS=-lfl

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS)
	@echo "all lexer tests pass"

$(TESTS): test
	@./test < $@.txt > $@.out 2> $@.stderr
	@cat $@.stderr >> $@.out; rm $@.stderr
	@$(DIFF) $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm $@.out $@.result

