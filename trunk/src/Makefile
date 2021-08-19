#	Makefile for fsmlang
#

.SECONDEXPANSION:

CFLAGS += -I ../src -I .

ARCHITECTURES=Cygwin \
	Mingw        \
	Mingwsa

Cygwin Cygwin.clean Cygwin.test Cygwin.testonly Cygwin.lexer Cygwin.parser : OUT_DIR=../cygwin
Mingw  Mingw.clean  Mingw.test  Mingw.testonly Mingw.lexer  Mingw.parser  : OUT_DIR=../mingw
Mingwsa  Mingwsa.clean  Mingwsa.test  Mingwsa.testonly Mingwsa.lexer  Mingwsa.parser  : OUT_DIR=../mingwsa

Cygwin Cygwin.clean Cygwin.test Cygwin.testonly Cygwin.lexer Cygwin.parser : ARCH=CYGWIN
Mingw  Mingw.clean  Mingw.test  Mingw.testonly Mingw.lexer  Mingw.parser  : ARCH=CYGWIN
Mingwsa  Mingwsa.clean  Mingwsa.test  Mingwsa.testonly Mingwsa.lexer  Mingwsa.parser  : ARCH=CYGWIN

all:
	@echo You must choose a target.

$(ARCHITECTURES):
	$(MAKE) CFLAGS="$(CFLAGS)" -C $(OUT_DIR) fsm

$(addsuffix .clean, $(ARCHITECTURES)):
	$(MAKE) -C $(OUT_DIR) clean
	$(MAKE) -C ../test clean

$(addsuffix .test, $(ARCHITECTURES)): $$(basename $$@)
	$(MAKE) -C $(OUT_DIR) ARCH=$(ARCH) test

$(addsuffix .testonly, $(ARCHITECTURES)): $$(basename $$@)
	$(MAKE) -C $(OUT_DIR) ARCH=$(ARCH) testonly

$(addsuffix .lexer, $(ARCHITECTURES)):
	$(MAKE) -C ../test lexer

$(addsuffix .parser, $(ARCHITECTURES)):
	$(MAKE) ARCH=$(ARCH) -C ../test parser

