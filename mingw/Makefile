# Makefile for the FSM Description Language
#
#

VPATH=../src

OUTPUT_DIR != PWD


#Bad, all three of these are supposed to be applications from C:\cygwin\bin but there is no checking to 
#make sure that that's what we are using.
LEX = flex -I
YACC = bison -d -y
CC = i686-w64-mingw32-gcc.exe

override CFLAGS += -DCYGWIN -DMINGW
CFLAGS += -DFSM_DEBUG

LDFLAGS += -L /usr/lib

.SUFFIXES: .fsm .c

SRC =	lexer.c \
			parser.c \
			revision.c \
			rindex.c \
			fsm_utils.c \
			fsm_html.c	\
			fsm_c.c

include ../depends.mk

all: fsm

clean::
	-@rm -f y.tab.[ch] *.o core 2> /dev/nul
	-@rm -f parser.c lexer.c    2> /dev/nul
	-@rm -f *.d* *.exe          2> /dev/nul
	-@rm -f *.stackdump         2> /dev/nul

test: 
	$(MAKE) -C ../test clean
	$(MAKE) CFLAGS="-DCYGWIN -DMINGW" OUTPUT_DIR="$(OUTPUT_DIR)" -C ../test all

lexer.o fsm_utils.o: y.tab.h

y.tab.h:	parser.o

fsm:	$(SRC:.c=.o)
	$(CC) -o $@ $(LDFLAGS) $(SRC:.c=.o) -lfl


