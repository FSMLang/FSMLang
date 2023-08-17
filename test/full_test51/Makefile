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

TARGET = check_html

include ../test.mk
include ../../fsmrules.mk

check_html: comment_test.html sub1.html
	$(DIFF) comment_test.html comment_test.html.canonical > test.result
	$(DIFF) sub1.html sub1.html.canonical > test.result
	@rm test.result
	@rm *.html

