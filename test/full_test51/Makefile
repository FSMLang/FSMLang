##########################################
#
# makefile for individual test
#
#

override CFLAGS += -DFSM_DEBUG \
                   -DNEW_MACHINE_DEBUG \
                   -DSUB_MACHINE1_DEBUG \
                   -DSUB_MACHINE2_DEBUG \
                   -I../

override FSM_HTML_FLAGS += --css-content-filename=../../fsmlang.css

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")

SUB_MACHINES=sub1

TARGET = check_html
TEST_INPUT = $(addsuffix .html, $(SUB_MACHINES)) $(addsuffix .plantuml, $(SUB_MACHINES)) $(FSM_SRC:.fsm=.html) $(FSM_SRC:.fsm=.plantuml)

include ../test.mk
include ../../fsmrules.mk

check_html: $(TEST_INPUT)
	@echo $(FSM_SRC:.fsm=.html)
	@echo $(FSM_SRC:.fsm=.plantuml)
	@echo $(TEST_INPUT)
	$(DIFF) comment_test.html comment_test.html.canonical > test.result
	$(DIFF) sub1.html sub1.html.canonical > test.result
	$(DIFF) comment_test.plantuml comment_test.plantuml.canonical > test.result
	$(DIFF) sub1.plantuml sub1.plantuml.canonical > test.result
	@rm test.result
	@rm *.html
	@rm *.plantuml

$(addsuffix .html, $(SUB_MACHINES)) : $(FSM_SRC)
$(addsuffix .plantuml, $(SUB_MACHINES)) : $(FSM_SRC)
