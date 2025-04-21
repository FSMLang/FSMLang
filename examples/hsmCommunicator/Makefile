##########################################
#
# makefile for individual test
#
#

OUTPUT_DIR=

override CFLAGS += -I../           \
	 		-Wall                     \
			-Wpedantic                \
			-Wextra                   \
	 		-ggdb                     \
         -DHSM_COMMUNICATOR_DEBUG  \
         -DESTABLISH_SESSION_DEBUG \
         -DSEND_MESSAGE_DEBUG


override FSM_FLAGS += --generate-weak-fns=false

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

#action functions are placed in files ending in "-actions.c"
SRC += $(shell find . -name "*-actions.c" -printf "%P ")

include ../../fsmrules.mk
include ../../depends.mk

OBJS = $(SRC:.c=.o) $(GENERATED_SRC:.c=.o)

all: hsmCommunicator.exe

run: hsmCommunicator.exe
	@./$<

do_check: hsmCommunicator.exe
	@./$< > out
	@diff out canonical > result

check: do_check clean

hsmCommunicator.exe: $(OBJS) Makefile
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean::
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

stats.txt: $(FSM_SRC)
	@$(FSM) -s $< > $@

