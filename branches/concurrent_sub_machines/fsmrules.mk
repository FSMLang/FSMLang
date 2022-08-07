####################################################
#
#  .fsm rules
#

.SUFFIXES: .fsm .html

ifdef OUTPUT_DIR
FSM=$(OUTPUT_DIR)/fsm
endif

FSM ?= fsm

.fsm.o:
	@echo "FSM:" $(FSM) $(FSM_FLAGS)
	$(FSM) $< > fsmout
	$(CC) -c $(CFLAGS) $*.c
	rm $*.c

.fsm.c:
	@echo "FSM:" $(FSM)
	$(FSM) $(FSM_FLAGS) $< > fsmout

.fsm.h:
	@echo "FSM:" $(FSM)
	$(FSM) $(FSM_FLAGS) $< > fsmout

.fsm.html:
	@echo "FSM:" $(FSM)
	$(FSM) $(FSM_FLAGS) -th $< > fsmout

$(FSM_SRC:.fsm=.html): $(FSM_SRC) $(FSM)

$(FSM_SRC:.fsm=.h): $(FSM_SRC) $(FSM)

$(FSM_SRC:.fsm=.c): $(FSM_SRC) $(FSM)

$(SRC): $(FSM_SRC:.fsm=.h)

