##########################################
#
# makefile for individual test
#
#

ifndef OUTPUT_DIR
$(error OUTPUT_DIR must be defined as the location of the created executable.)
endif
include $(OUTPUT_DIR)/system.mk
ifdef CFG
include $(OUTPUT_DIR)/../cfg-$(CFG).mk
endif

VPATH=../../src

override CFLAGS += -DLEX_DEBUG -I../ -I$(VPATH)

SRC = $(LEXER_SRC)       \
			lexer_debug.c \
			list.c \
			ancestry.c \
			fsm_utils.c

TESTS = $(basename $(shell find . -name "lexer-test*.txt" -printf "%P "))

ifneq (,$(findstring SUPPORT_INCLUDE_FILES, $(LEXER_FEATURES)))
TESTS += $(basename $(shell find . -name "lexer-v2-test*.txt" -printf "%P "))
endif

LDFLAGS=-lfl

NO_RUNTEST=TRUE
include ../test.mk

runtest: $(TESTS) clean
	@echo "all lexer tests pass"

$(TESTS): test
ifneq (,$(findstring SUPPORT_INCLUDE_FILES, $(LEXER_FEATURES)))
	@./test $@.txt > $@.out 2> $@.stderr
else
	@./test < $@.txt > $@.out 2> $@.stderr
endif
	@cat $@.stderr >> $@.out; rm -f $@.stderr
	@$(DIFF) $@.out $@.canonical > $@.result
	@echo $@ successful
	@rm -f $@.out $@.result

