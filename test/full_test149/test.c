#include "test_fsm.h"

int main(void)
{
	TEST_FSM_EVENT e;

	e.event = THIS(e1);

	e.event_data.e1_data.i = 1;
	e.event_data.e1_data.ext_event = THIS(e1);
	run_test_fsm(&e);
	run_test_fsm(&e);
	run_test_fsm(&e);
	run_test_fsm(&e);

	e.event_data.e1_data.ext_event = THIS(e2);
	run_test_fsm(&e);

	return 0;
}
