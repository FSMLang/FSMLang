
.PHONY: test_file_listers test_m_c test_m_s test_md_c test_md_s test_mh_c test_mh_s

test_m_c: FSM_FLAGS=-M -tc
test_m_s: FSM_FLAGS=-M -ts
test_md_c: FSM_FLAGS=-Md -tc
test_md_s: FSM_FLAGS=-Md -ts
test_mh_c: FSM_FLAGS=-Mh -tc
test_mh_s: FSM_FLAGS=-Mh -ts

FSM=$(OUTPUT_DIR)/fsm

TARGET=diff_canonical

test_file_listers: test_m_c test_m_s test_md_c test_md_s test_mh_c test_mh_s
	for f in `ls *.out`; do diff $$f $${f%.*}.canonical > $${f%.*}.result; done
	-rm *.out
	-rm *.result

test_m_c test_m_s test_md_c test_md_s test_mh_c test_mh_s:
	$(FSM) $(FSM_FLAGS) test.fsm > $@.out

clean::
	-@rm *.out 2> /dev/null
	-@rm *.result 2> /dev/null
