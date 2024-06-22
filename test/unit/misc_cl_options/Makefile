
.PHONY: test_file_listers test_m_c test_m_s test_md_c test_md_s test_mh_c test_mh_s test_file_creators

test.json: FSM_FLAGS=--event-cross-ref-only=true
test.xml: FSM_FLAGS=--event-cross-ref-format=xml
test.tab: FSM_FLAGS=--event-cross-ref-format=tab
test.csv: FSM_FLAGS=--event-cross-ref-format=csv

FSM=$(OUTPUT_DIR)/fsm

all: test_file_creators

runtest: all

test_file_creators: test.json test.xml test.tab
	@rm $?
	@rm -f *.result

test.json test.xml test.tab test.csv: $(FSM) Makefile
	$(FSM) $(FSM_FLAGS) test.fsm 
	@diff $@ $@.canonical > $@.result

clean::
	@rm -f *.out 2> /dev/null
	@rm -f *.result 2> /dev/null
	-@rm -f test.json 2> /dev/null
	-@rm -f test.csv 2> /dev/null
	-@rm -f test.tab 2> /dev/null
	-@rm -f test.xml 2> /dev/null

