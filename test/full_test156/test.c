#include <stdio.h>
#include "test_fsm.h"

int main(void)
{
	TEST_FSM_EVENT e;

	e.event = THIS(e0);
	run_test_fsm(&e);

	e.event = THIS(e1);
	e.event_data.e1_data.i = 1;

	run_test_fsm(&e);
	run_test_fsm(&e);
	run_test_fsm(&e);
	run_test_fsm(&e);

	e.event = THIS(e2);
	run_test_fsm(&e);

	e.event = THIS(e1);
	run_test_fsm(&e);

	e.event = THIS(e2);
	run_test_fsm(&e);

	e.event = THIS(e3);
	run_test_fsm(&e);

	e.event = THIS(e4);
	run_test_fsm(&e);

	e.event = THIS(e3);
	run_test_fsm(&e);

	return 0;
}

