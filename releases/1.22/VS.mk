#makefile for the Visual Studio build of fsm

TOOLS=$(CLEARCASE_ROOT)\PMAComps\Tools\bin

LEX = $(TOOLS)\flex
YACC = $(TOOLS)\bison.exe 

CC = cl.exe
LIBC = lib.exe
LINK = link.exe

.SUFFIXES: .c .y .l .obj

BISON_SIMPLE := vs.bison.custom

OBJS=lexer.obj \
	fsm_utils.obj  \
	parser.obj \
	revision.obj \
	fsm_c.obj \
	fsm_html.obj \
	getopta.obj \
	rindex.obj

C_FLAGS = -Zi -Od -DDEBUG -DMSDOS -DVS
LD_FLAGS = -nologo -Debug -INCREMENTAL:NO
LIB_FLAGS = -nologo

#FLEX_DEBUG_MODE=-d
FLEX_NO_DEFAULT_RULE=-s
FLEX_TABLE_COMPRESSION=-Cem

L_ARG=-B $(FLEX_DEBUG_MODE) $(FLEX_NO_DEFAULT_RULE) $(FLEX_TABLE_COMPRESSION)

all:	wfsm.exe

wfsm.exe:	$(OBJS) VS.mk
	$(LINK) $(LD_FLAGS) -OUT:$@ $(OBJS) 

clean:
	-erase *.obj parser.h lex.yy.c y.tab.c lexer.c parser.c
	-erase *.exe *.mib wfsm.pdb vc60.pdb

install: wfsm.exe
	copy wfsm.exe ..\bin

.c.obj:
	$(CC) $(C_FLAGS) -c -o$*.obj $<

.y.c:
	set BISON_SIMPLE=$(BISON_SIMPLE) &\
	$(YACC) $(Y_ARG) -d -o $@ $(subst \,/,$<)

.l.c:
	$(LEX) $(L_ARG) -o$(@F) $<

lexer.obj: VS.mk fsm_priv.h lexer.c

lexer.c: VS.mk fsm_priv.h parser.h lexer.l

parser.h: parser.obj VS.mk parser.y

fsm_utils.obj: VS.mk fsm_priv.h

parser.obj: VS.mk fsm_priv.h fsm_html.h parser.c

parser.c: VS.mk fsm_priv.h fsm_html.h parser.y

fsm_html.obj: VS.mk fsm_priv.h fsm_html.h

fsm_cpp.obj: VS.mk fsm_priv.h fsm_cpp.h
