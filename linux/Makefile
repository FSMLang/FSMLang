# Makefile for the FSM Description Language
#
#

VPATH=../src

OUTPUT_DIR = $(shell pwd)

ifdef CFG
include ../cfg-$(CFG).mk
endif
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
	-@rm -f y.output            2> /dev/null

test: 
	$(MAKE) OUTPUT_DIR="$(OUTPUT_DIR)" -C ../test clean
	$(MAKE) ARCH="$(ARCH)" OUTPUT_DIR="$(OUTPUT_DIR)" -C ../test all

testonly:
	$(MAKE) ARCH="$(ARCH)" OUTPUT_DIR="$(OUTPUT_DIR)" -C ../test all

lexer.o fsm_utils.o: y.tab.h

y.tab.h:	parser.o

fsm:	$(SRC:.c=.o)
	echo $(CFLAGS)
	$(CC) -o $@ $(LDFLAGS) $(SRC:.c=.o) -lfl


