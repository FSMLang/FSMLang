####################################################
#
#  .fsm rules
#

.SUFFIXES: .fsm .html .plantuml 

ifdef OUTPUT_DIR
FSM=$(OUTPUT_DIR)/fsm
endif

ifeq ($(WEAK_FNS),FALSE)
FSM_FLAGS += --generate-weak-fns=false
endif

FSM ?= fsm

ifeq ($(TARGET),fsm_fail_is_pass)
CALL_FSM_FAILURE_A_SUCCESS = ; if [ $$? -ne 0 ]; then echo "expected fsm failure; test passes"; true; else echo "did not find an expected fsm failure; test fails"; false; fi
endif

GENERATED_SRC = $(shell $(FSM) -M -tc $(FSM_SRC))
#GENERATED_HEADERS = $(GENERATED_SRC:.c=_priv.h)
#GENERATED_HEADERS += $(FSM_SRC:.fsm=.h)

.fsm.o:
	@echo "FSM:" $(FSM) $(FSM_FLAGS)
	$(FSM) $(FSM_FLAGS) $< > fsmout $(CALL_FSM_FAILURE_A_SUCCESS)
	$(CC) -c $(CFLAGS) $*.c
	rm $*.c

.fsm.c:
	@echo "FSM:" $(FSM)
	$(FSM) $(FSM_FLAGS) $< > fsmout $(CALL_FSM_FAILURE_A_SUCCESS)

.fsm.h:
	@echo "FSM:" $(FSM)
	$(FSM) $(FSM_FLAGS) $< > fsmout $(CALL_FSM_FAILURE_A_SUCCESS)

.fsm.html:
	@echo "FSM:" $(FSM)
	$(FSM) $(FSM_HTML_FLAGS) -th $< > fsmout $(CALL_FSM_FAILURE_A_SUCCESS)

.fsm.plantuml:
	@echo "FSM:" $(FSM)
	$(FSM) $(FSM_PLANTUML_FLAGS) -tp $< > fsmout $(CALL_FSM_FAILURE_A_SUCCESS)

$(FSM_SRC:.fsm=.html): $(FSM_SRC) $(FSM)

$(FSM_SRC:.fsm=.h): $(FSM_SRC) $(FSM)

$(FSM_SRC:.fsm=.c): $(FSM_SRC) $(FSM)

$(FSM_SRC:.fsm=.plantuml): $(FSM_SRC) $(FSM)

$(SRC): $(FSM_SRC:.fsm=.h)

