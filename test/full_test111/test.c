#include "test_fsm.h"

static TEST_FSM test_fsm =
{
	.data = {.e1_count = 0}
	, .state = test_fsm_s1
	, .actionArray = &THIS(action_array)
	, .fsm         = test_fsmFSM
};

static pTEST_FSM ptest_fsm = &test_fsm;

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
