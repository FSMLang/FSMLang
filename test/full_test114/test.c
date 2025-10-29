#include "test_fsm.h"

#define INIT_FSM_DATA { .e1_count = 0 }
#define INIT_FSM_DATA_SUB { .sub_e1_count = 0 }
TEST_FSM_INSTANCE(test_fsm, INIT_FSM_DATA, INIT_FSM_DATA_SUB);

static void run_test_fsm(pTEST_FSM_EVENT pevent)
{
	ptest_fsm->fsm(ptest_fsm, pevent);
}

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
