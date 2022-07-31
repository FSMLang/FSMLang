#	Makefile for fsmlang
#

.SECONDEXPANSION:

CFLAGS += -I ../src -I .

ARCHITECTURES=Cygwin \
	Mingw        \
	Mingwsa      \
	Linux

Cygwin Cygwin.clean Cygwin.test Cygwin.testonly Cygwin.lexer Cygwin.parser : OUT_DIR=../cygwin
Mingw  Mingw.clean  Mingw.test  Mingw.testonly Mingw.lexer  Mingw.parser  : OUT_DIR=../mingw
Mingwsa  Mingwsa.clean  Mingwsa.test  Mingwsa.testonly Mingwsa.lexer  Mingwsa.parser  : OUT_DIR=../mingwsa
Linux  Linux.clean  Linux.test  Linux.testonly Linux.lexer  Linux.parser  : OUT_DIR=../linux

Cygwin Cygwin.clean Cygwin.test Cygwin.testonly Cygwin.lexer Cygwin.parser : ARCH=CYGWIN
Mingw  Mingw.clean  Mingw.test  Mingw.testonly Mingw.lexer  Mingw.parser  : ARCH=CYGWIN
Mingwsa  Mingwsa.clean  Mingwsa.test  Mingwsa.testonly Mingwsa.lexer  Mingwsa.parser  : ARCH=CYGWIN
Linux  Linux.clean  Linux.test  Linux.testonly Linux.lexer  Linux.parser  : ARCH=LINUX

Mingwsa  Mingwsa.clean  Mingwsa.test  Mingwsa.testonly Mingwsa.lexer  Mingwsa.parser  : NEED_RINDEX=TRUE

all:
	@echo You must choose a target.

$(ARCHITECTURES):
	$(MAKE) CFLAGS="$(CFLAGS)" -C $(OUT_DIR) fsm

$(addsuffix .clean, $(ARCHITECTURES)):
	$(MAKE) -C $(OUT_DIR) clean
	$(MAKE) -C ../test clean

$(addsuffix .test, $(ARCHITECTURES)): $$(basename $$@)
	$(MAKE) -C $(OUT_DIR) ARCH=$(ARCH) NEED_RINDEX=$(NEED_RINDEX) test

$(addsuffix .testonly, $(ARCHITECTURES)): $$(basename $$@)
	$(MAKE) -C $(OUT_DIR) ARCH=$(ARCH) NEED_RINDEX=$(NEED_RINDEX) testonly

$(addsuffix .lexer, $(ARCHITECTURES)):
	$(MAKE) NEED_RINDEX=$(NEED_RINDEX) -C ../test lexer

$(addsuffix .parser, $(ARCHITECTURES)):
	$(MAKE) ARCH=$(ARCH) NEED_RINDEX=$(NEED_RINDEX) -C ../test parser

