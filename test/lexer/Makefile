##########################################
#
# makefile for individual test
#
#

ifndef OUTPUT_DIR
$(error OUTPUT_DIR must be defined as the location of the created executable.)
endif
include $(OUTPUT_DIR)/system.mk

VPATH=../../src

override CFLAGS += -DLEX_DEBUG -I../ -I$(VPATH)

SRC = lexer.c       \
			lexer_debug.c \
			list.c \
			ancestry.c \
			fsm_utils.c

TESTS = $(basename $(shell find . -name "lexer-*.txt" -printf "%P "))

LDFLAGS=-lfl

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS)
	@echo "all lexer tests pass"

$(TESTS): test
	@./test $@.txt > $@.out 2> $@.stderr
	@cat $@.stderr >> $@.out; rm -f $@.stderr
	@$(DIFF) $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm -f $@.out $@.result

