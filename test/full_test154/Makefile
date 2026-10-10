##########################################
#
# makefile for individual test
#
#

.PHONY: docs

override CFLAGS += -I../     \
	 -Wall                    \
	 -ggdb                    \
    -DCOMMUNICATOR_DEBUG     \
    -DCOMMANDS_DEBUG         \
    -DBLE_DEBUG              \
    -DFIRST_CONNECTION_DEBUG \
    -DAUTH1_DEBUG            \
    -DAUTH2_DEBUG            \
    -DSYNC_DEBUG             \
	 -DSTEADY_STATE_DEBUG


VARIANTS=s
#TARGET=pass

#include ../weak_variants.mk
include ../variants.mk

FSM_FLAGS+=--add-profiling-macros=true --profile-sub-fsms=true --short-debug-names --add-machine-name
FINAL_FSM_FLAGS=--force-generation-of-event-passing-actions=false

FSM_FLAGS += -i3

FSM_PLANTUML_FLAGS=
FSM_RST_FLAGS=--include-uml-objects --use-sphinx-scrollable-ext

include ../../fsmrules.mk

pass:


docs: $(GENERATED_RST) $(GENERATED_PLANTUML)
	mv test_fsm.rst index.rst
	$(MAKE) -f sphinx.mk html

clean_docs:
	@rm -f index.rst 2> /dev/null
	@rm -f $(GENERATED_RST) 2> /dev/null
	@rm -f $(GENERATED_PLANTUML) 2> /dev/null
	@$(MAKE) -f sphinx.mk clean

check_generated:
	@echo GENERATED_RST: $(GENERATED_RST)
	@echo GENERATED_PLANTUML: $(GENERATED_PLANTUML)

