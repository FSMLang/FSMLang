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

ifndef FSM_SRC
FSM_SRC=$(wildcard *.fsm)
endif

ifeq ($(TARGET),fsm_fail_is_pass)
CALL_FSM_FAILURE_A_SUCCESS = ; if [ $$? -ne 0 ]; then echo "expected fsm failure; test passes"; true; else echo "did not find an expected fsm failure; test fails"; false; fi
endif

GENERATED_SRC = $(shell $(FSM) -M $(FSM_FLAGS) $(FSM_SRC))
GENERATED_HDR = $(shell $(FSM) -Mh $(FSM_FLAGS) $(FSM_SRC))

ifdef FSM_PLANTUML_FLAGS
GENERATED_PLANTUML = $(shell $(FSM) -tp -M $(FSM_PLANTUML_FLAGS) $(FSM_SRC))
endif

ifdef FSM_HTML_FLAGS
GENERATED_HTML = $(shell $(FSM) -th -M $(FSM_HTML_FLAGS) $(FSM_SRC))
endif

cleanfsm:
	@-rm -f $(GENERATED_SRC) 2> /dev/null
	@-rm -f $(GENERATED_HDR) 2> /dev/null

.fsm.o:
	@echo "FSM:" $(FSM) $(FSM_FLAGS)
	$(FSM) $(FSM_FLAGS) $< > fsmout $(CALL_FSM_FAILURE_A_SUCCESS)
	$(CC) -c $(CFLAGS) $*.c
	rm -f $*.c

.fsm.c:
	@echo "FSM:" $(FSM) $(FSM_FLAGS)
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

%.fsmd: %.fsm
	@set -e; $(FSM) -Md $(FSM_FLAGS) $< > $@

%.fsmdp: %.fsm
	@set -e; $(FSM) -tp -Md $(FSM_PLANTUML_FLAGS) $< > $@

%.fsmdh: %.fsm
	@set -e; $(FSM) -th -Md $(FSM_HTML_FLAGS) $< > $@

ifneq ($(TARGET),fsm_fail_is_pass)
ifneq ($(MAKECMDGOALS),clean)
ifdef FSM_PLANTUML_FLAGS
-include $(FSM_SRC:.fsm=.fsmdp)
else ifdef FSM_HTML_FLAGS
-include $(FSM_SRC:.fsm=.fsmdh)
else
-include $(FSM_SRC:.fsm=.fsmd)
endif
endif
endif
