# Makefile for the FSM Description Language
#
#

VPATH=../src

OUTPUT_DIR = $(shell pwd)


include system.mk

override CFLAGS += -DLINUX
CFLAGS += -DFSM_DEBUG

.SUFFIXES: .fsm .c

include source.mk
include ../depends.mk

all: fsm

clean::
	@echo cleaning $(OUTPUT_DIR)
	-@rm -f y.tab.[ch] *.o core 2> /dev/null
	-@rm -f parser.c lexer.c    2> /dev/null
	-@rm -f *.d*                2> /dev/null
	-@rm -f *.stackdump         2> /dev/null
	-@rm -f fsm                 2> /dev/null

test: 
	$(MAKE) -C ../test clean
	$(MAKE) CFLAGS="-DLINUX" OUTPUT_DIR="$(OUTPUT_DIR)" -C ../test all

testonly:
	$(MAKE) CFLAGS="-DCYGWIN" OUTPUT_DIR="$(OUTPUT_DIR)" -C ../test all

lexer.o fsm_utils.o: y.tab.h

y.tab.h:	parser.o

fsm:	$(SRC:.c=.o)
	echo $(CFLAGS)
	$(CC) -o $@ $(LDFLAGS) $(SRC:.c=.o) -lfl


