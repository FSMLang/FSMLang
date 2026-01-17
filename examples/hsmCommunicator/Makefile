##########################################
#
# makefile for hsmCommunicator example
#
# "make run" will build and run.
#

.PHONY: docs

OUTPUT_DIR=

override CFLAGS += -I../          \
	-Wall                     \
	-Wpedantic                \
	-Wextra                   \
	-ggdb                     \
	-DHSM_COMMUNICATOR_DEBUG  \
	-DESTABLISH_SESSION_DEBUG \
	-DSEND_MESSAGE_DEBUG


override FSM_FLAGS += -ts --generate-weak-fns=false

FSM_RST_FLAGS=--include-uml-objects

SPHINX_DIR ?= sphinx

FSM_SRC = $(shell find . -maxdepth 1 -name "*.fsm" -printf "%P ")

#action functions are placed in files ending in "-actions.c"
SRC += $(shell find . -maxdepth 1 -name "*-actions.c" -printf "%P ")

include ../../fsmrules.mk
include ../../depends.mk

OBJS = $(SRC:.c=.o) $(GENERATED_SRC:.c=.o)

check: hsmCommunicator.exe

run: hsmCommunicator.exe
	@./$<

do_check: hsmCommunicator.exe
	@./$< > out
	@diff out canonical > result

check: do_check clean

hsmCommunicator.exe: $(OBJS) Makefile
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:: clean_sphinx
	-@rm -f *.exe                          2> /dev/null
	-@rm -f out                            2> /dev/null
	-@rm -f result                         2> /dev/null
	-@rm -f fsmout                         2> /dev/null
	-@rm -f *.d*                           2> /dev/null
	-@rm -f *.fsmd*                        2> /dev/null
	-@rm -f *.o                            2> /dev/null
	-@rm -f *.stackdump                    2> /dev/null
	-@rm -f *.txt                          2> /dev/null
	-@rm -f $(GENERATED_SRC)               2> /dev/null
	-@rm -f $(GENERATED_HDR)               2> /dev/null
	-@rm -f $(GENERATED_PLANTUML)          2> /dev/null

clean_sphinx:
	@$(MAKE) -C $(SPHINX_DIR) clean
	@cd $(SPHINX_DIR) && rm -f $(GENERATED_RST) $(GENERATED_PLANTUML) 2> /dev/null
	@cd $(SPHINX_DIR) && rm -f *.svg 2> /dev/null
	@cd $(SPHINX_DIR)/_static && rm -f *.svg 2> /dev/null

stats.txt: $(FSM_SRC)

docs: $(GENERATED_RST) $(GENERATED_PLANTUML)
	@mv $(GENERATED_RST) $(SPHINX_DIR)
	@mv $(GENERATED_PLANTUML) $(SPHINX_DIR)
	@$(MAKE) -C $(SPHINX_DIR) html

