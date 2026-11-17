#include "test_fsm_priv.h"

TEST_FSM_EVENT_ENUM UFMN(a1)(pTEST_FSM pfsm)
{
	DBG_PRINTF("%s", __func__);
	printf("i: %d; j: %d; k: %d; l: %d\n"
				, pfsm->data.i, pfsm->data.j, pfsm->data.k, pfsm->data.l
			);
	return THIS(noEvent);
}

void test_fsm_xlate_e1(pTEST_FSM pfsm, pTEST_FSM_EVENT pevent)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pe1_data = &pevent->event_data.e1_data;

	(void) run_test_fsm_e1_translator(pfsm, pevent);
}

