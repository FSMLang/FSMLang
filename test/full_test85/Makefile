##########################################
#
# makefile for individual test
#
#
.PHONEY: check_output

override CFLAGS += -DANOTHER_NEW_MACHINE_DEBUG                                               \
                   -DANOTHER_NEW_MACHINE_ANOTHER_SUB_MACHINE1_DEBUG                          \
                   -DANOTHER_NEW_MACHINE_ANOTHER_SUB_MACHINE1_ANOTHER_SUB_SUB_MACHINE1_DEBUG \
                   -DNEW_MACHINE_DEBUG                                                       \
                   -DNEW_MACHINE_SUB_MACHINE1_DEBUG                                          \
                   -DNEW_MACHINE_SUB_MACHINE1_SUB_SUB_MACHINE1_DEBUG 

FSM_FLAGS = -te 

FSM_SRC = $(shell find . -name "*.fsm" -printf "%P ")
SRC = $(shell find . -name "*actions.c" -printf "%P ")
SRC += test.c

include ../../fsmrules.mk

#TARGET     = check_objects

include ../test.mk

check_objects:
	@echo $(OBJS)
	@echo $(SRC)
	@echo $(FSM_SRC)
	@echo $(GENERATED_SRC)

