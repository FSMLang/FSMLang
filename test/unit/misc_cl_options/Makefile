
.PHONY: test_file_listers test_m_c test_m_s test_md_c test_md_s test_mh_c test_mh_s test_file_creators

test_m_c: FSM_FLAGS=-M -tc
test_m_s: FSM_FLAGS=-M -ts
test_md_c: FSM_FLAGS=-Md -tc
test_md_s: FSM_FLAGS=-Md -ts
test_mh_c: FSM_FLAGS=-Mh -tc
test_mh_s: FSM_FLAGS=-Mh -ts
test_m_p: FSM_FLAGS=-M -tp
test_m_h: FSM_FLAGS=-M -th
test.json: FSM_FLAGS=--event-cross-ref-only=true
test.xml: FSM_FLAGS=--event-cross-ref-format=xml
test.tab: FSM_FLAGS=--event-cross-ref-format=tab
test.csv: FSM_FLAGS=--event-cross-ref-format=csv

FSM=$(OUTPUT_DIR)/fsm

all: test_file_listers test_file_creators

test_file_creators: test.json test.xml test.tab
	@rm $?
	@rm -f *.result

test_file_listers: test_m_c test_m_s test_md_c test_md_s test_mh_c test_mh_s test_m_p test_m_h
	@rm -f *.out
	@rm -f *.result

test_m_c test_m_s test_md_c test_md_s test_mh_c test_mh_s test_m_p test_m_h test_xref_json: $(FSM) Makefile
	$(FSM) $(FSM_FLAGS) test.fsm > $@.out
	@diff $@.out $@.canonical > $@.result

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

