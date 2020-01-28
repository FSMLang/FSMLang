#	Makefile for fsmlang
#

.SECONDEXPANSION:

CFLAGS += -I ../src -I .

ARCHITECTURES=Cygwin \
	Mingw

Cygwin Cygwin.clean Cygwin.test Cygwin.lexer Cygwin.parser : OUT_DIR=../cygwin
Mingw  Mingw.clean  Mingw.test  Mingw.lexer  Mingw.parser  : OUT_DIR=../mingw

Cygwin Cygwin.clean Cygwin.test Cygwin.lexer Cygwin.parser : ARCH=CYGWIN
Mingw  Mingw.clean  Mingw.test  Mingw.lexer  Mingw.parser  : ARCH=CYGWIN

all:
	@echo You must choose a target.

$(ARCHITECTURES):
	$(MAKE) CFLAGS="$(CFLAGS)" -C $(OUT_DIR) fsm

$(addsuffix .clean, $(ARCHITECTURES)):
	$(MAKE) -C $(OUT_DIR) clean
	$(MAKE) -C ../test clean

$(addsuffix .test, $(ARCHITECTURES)): $$(basename $$@)
	$(MAKE) -C $(OUT_DIR) ARCH=$(ARCH) test

$(addsuffix .lexer, $(ARCHITECTURES)):
	$(MAKE) -C ../test lexer

$(addsuffix .parser, $(ARCHITECTURES)):
	$(MAKE) ARCH=$(ARCH) -C ../test parser

