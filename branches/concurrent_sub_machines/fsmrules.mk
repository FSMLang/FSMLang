####################################################
#
#  .fsm rules
#

.SUFFIXES: .fsm .html

ifdef OUTPUT_DIR
FSM=$(OUTPUT_DIR)/fsm $(FSM_FLAGS)
endif

FSM ?= fsm $(FSM_FLAGS)

.fsm.o:
	@echo "FSM:" $(FSM)
	$(FSM) $< > fsmout
	$(CC) -c $(CFLAGS) $*.c
	rm $*.c

.fsm.c:
	@echo "FSM:" $(FSM)
	$(FSM) $< > fsmout

.fsm.h:
	@echo "FSM:" $(FSM)
	$(FSM) $< > fsmout

.fsm.html:
	@echo "FSM:" $(FSM)
	$(FSM) -th $< > fsmout

$(FSM_SRC:.fsm=.h): $(FSM_SRC)

$(FSM_SRC:.fsm=.c): $(FSM_SRC)

$(SRC): $(FSM_SRC:.fsm=.h)

