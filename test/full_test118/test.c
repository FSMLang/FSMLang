#include "test_fsm.h"

int main(void)
{
	TEST_FSM_EVENT e;

	e.event = THIS(e1);
	e.event_data.e1_data.i = 1;
	run_test_fsm(&e);
	run_test_fsm(&e);
	run_test_fsm(&e);
	run_test_fsm(&e);

	return 0;
}
