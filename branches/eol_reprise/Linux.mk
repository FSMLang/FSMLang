# Makefile for the FSM Description Language

LEX = flex -I
YACC = bison -d -y
CC = gcc -g

C_FLAGS += -DLINUX 
LIBS = -lfl

.SUFFIXES: .fsm .c .y .l .o

BISON_SIMPLE := vs.bison.custom

LEXER_TEST_OBJS = lexer.o	\
									fsm_utils.o

FSM_OBJS =	lexer.o \
						parser.o \
						revision.o \
						fsm_utils.o \
						fsm_html.o	\
						fsm_c.o

FSM_C_TEST_OBJS = fsm_c.o\
									fsm_utils.o

FULL_TEST1_OBJS = parser-test2.o \
									pt2-actions.o

FULL_TEST2_OBJS = parser-test4.o \
									pt4-actions.o

all: fsm

install: fsm
	cp $< $(HOME)/bin

test: full_test 

clean:
	-rm -f y.tab.h *.o core;
	-rm -f *.result lexer parser fsm;
	-rm -f full_test*

full_test: parser-test lexer-test full_test1 full_test2
	./evaluateTests full_test1
	./evaluateTests full_test2

full_test1: fsm $(FULL_TEST1_OBJS)
	$(CC) -o $@ $(LDFLAGS) $(FULL_TEST1_OBJS) $(LIBS)
	- rm -rf $(FULL_TEST1_OBJS);

full_test2: fsm $(FULL_TEST2_OBJS)
	$(CC) -o $@ $(LDFLAGS) $(FULL_TEST2_OBJS) $(LIBS)
	- rm -rf $(FULL_TEST2_OBJS);

fsm_c_test:
	- rm -f $(FSM_C_TEST_OBJS);
	$(MAKE) -f Linux.mk fsm_c CFLAGS=-DFSM_C_TEST;
	- rm foo.*
	- ./fsm_c
	- cat foo.c
	- rm -f $(FSM_C_TEST_OBJS);

lexer-test:
	- rm -f $(LEXER_TEST_OBJS)
	$(MAKE) -f Linux.mk lexer C_FLAGS="-DLEX_DEBUG -DLINUX";
	./evaluateTests lexer;
	- rm -f $(LEXER_TEST_OBJS)

parser-test:
	- rm -f $(FSM_OBJS)
	$(MAKE) -f Linux.mk parser "C_FLAGS=-DPARSER_DEBUG -DLINUX";
	./evaluateTests parser;
	- rm -f $(FSM_OBJS)

#this target is for testing the lexer only
lexer:	$(LEXER_TEST_OBJS)
	$(CC) -o $@ $(LDFLAGS) $(LEXER_TEST_OBJS) $(LIBS)

#this target is for testing the fsm_c module only
fsm_c:	$(FSM_C_TEST_OBJS)
	$(CC) -o $@ $(LDFLAGS) $(FSM_C_TEST_OBJS)

fsm:	$(FSM_OBJS) Linux.mk
	$(CC) -o $@ $(LDFLAGS) $(FSM_OBJS) $(LIBS)

parser:	$(FSM_OBJS)
	$(CC) -o $@ $(LDFLAGS) $(FSM_OBJS) $(LIBS)

.fsm.o:
	./fsm $<
	$(CC) -c $(CFLAGS) $*.c
	rm $*.c

.fsm.c:
	./fsm $<


.c.o:
	$(CC) $(C_FLAGS) -c -o$*.o $<

.y.c:
	set BISON_SIMPLE=$(BISON_SIMPLE) &\
	$(YACC) $(Y_ARG) -d -o $@ $<

.l.c:
	$(LEX) $(L_ARG) -o$(@F) $<

lexer.o: Linux.mk fsm_priv.h lexer.c

lexer.c: Linux.mk fsm_priv.h parser.h lexer.l

parser.h: parser.o Linux.mk parser.y

fsm_utils.o: Linux.mk fsm_priv.h

parser.o: Linux.mk fsm_priv.h fsm_html.h parser.c

parser.c: Linux.mk fsm_priv.h fsm_html.h parser.y

fsm_html.o: Linux.mk fsm_priv.h fsm_html.h

fsm_cpp.o: Linux.mk fsm_priv.h fsm_cpp.h

pt2-actions.o:	parser-test2.h

parser-test2.h:	parser-test2.c

pt4-actions.o:	parser-test4.h

parser-test4.h:	parser-test4.c
