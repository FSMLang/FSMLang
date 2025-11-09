#include "test_fsm.h"

#define INIT_FSM_DATA { .e1_count = 0, .name="test_fsm" }
#define INIT_FSM_DATA_SUB { .sub_e1_count = 0 }
#define INIT_FSM_DATA_SUB2 { .sub2_e1_count = 0 }
#define INIT_FSM_DATA_SUB_SUB { .subSub_e1_count = 0 }
TEST_FSM_INSTANCE(test_fsm
						, INIT_FSM_DATA
						, INIT_FSM_DATA_SUB
						, INIT_FSM_DATA_SUB_SUB
						, INIT_FSM_DATA_SUB2
						, INIT_FSM_DATA_SUB_SUB
						);

#define INIT1_FSM_DATA { .e1_count = 100, .name="test_fsm1" }
#define INIT1_FSM_DATA_SUB { .sub_e1_count = 100 }
#define INIT1_FSM_DATA_SUB2 { .sub2_e1_count = 100 }
#define INIT1_FSM_DATA_SUB_SUB { .subSub_e1_count = 100 }
TEST_FSM_INSTANCE(test_fsm1
						, INIT1_FSM_DATA
						, INIT1_FSM_DATA_SUB
						, INIT1_FSM_DATA_SUB_SUB
						, INIT1_FSM_DATA_SUB2
						, INIT1_FSM_DATA_SUB_SUB
						);

#define INIT2_FSM_DATA { .e1_count = 1000, .name="test_fsm2" }
#define INIT2_FSM_DATA_SUB { .sub_e1_count = 1000 }
#define INIT2_FSM_DATA_SUB2 { .sub2_e1_count = 1000 }
#define INIT2_FSM_DATA_SUB_SUB { .subSub_e1_count = 1000 }
TEST_FSM_INSTANCE(test_fsm2
						, INIT2_FSM_DATA
						, INIT2_FSM_DATA_SUB
						, INIT2_FSM_DATA_SUB_SUB
						, INIT2_FSM_DATA_SUB2
						, INIT2_FSM_DATA_SUB_SUB
						);

static void run_test_fsm(pTEST_FSM_EVENT pevent)
{
	ptest_fsm->fsm(ptest_fsm, pevent);
}

static void run_test_fsm1(pTEST_FSM_EVENT pevent)
{
	ptest_fsm1->fsm(ptest_fsm1, pevent);
}

static void run_test_fsm2(pTEST_FSM_EVENT pevent)
{
	ptest_fsm2->fsm(ptest_fsm2, pevent);
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

	run_test_fsm1(&e);
	run_test_fsm1(&e);
	run_test_fsm1(&e);
	run_test_fsm1(&e);

	run_test_fsm2(&e);
	run_test_fsm2(&e);
	run_test_fsm2(&e);
	run_test_fsm2(&e);

	return 0;
}
