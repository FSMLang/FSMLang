# Makefile for the FSM Description Language
#
#

VPATH=../src

OUTPUT_DIR = $(shell pwd)

include system.mk

override CFLAGS += -DCYGWIN -DMINGW
CFLAGS += -DFSM_DEBUG

.SUFFIXES: .fsm .c

include source.mk
include ../depends.mk

all: fsm

clean::
	-@rm -f y.tab.[ch] *.o core 2> /dev/null
	-@rm -f parser.c lexer.c    2> /dev/null
	-@rm -f *.d* *.exe          2> /dev/null
	-@rm -f *.stackdump         2> /dev/null
	-@rm -f *.output            2> /dev/null

test: 
	@echo mingwsa/Makefile: OUTPUT_DIR: $(OUTPUT_DIR)
	$(MAKE) -C ../test clean
	$(MAKE) CFLAGS="-DCYGWIN -DMINGW" OUTPUT_DIR="$(OUTPUT_DIR)" -C ../test all

testonly: 
	$(MAKE) CFLAGS="-DCYGWIN -DMINGW" OUTPUT_DIR="$(OUTPUT_DIR)" -C ../test all

lexer.o fsm_utils.o: y.tab.h

y.tab.h:	parser.o

fsm:	$(SRC:.c=.o)
	$(CC) -o $@ $(LDFLAGS) $(SRC:.c=.o) -lfl


