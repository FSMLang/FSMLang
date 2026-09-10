#include "test_fsm_priv.h"

TEST_FSM_EVENT_ENUM UFMN(a1)(pTEST_FSM pfsm)
{
	DBG_PRINTF("%s", __func__);
	printf("i: %d; j: %d; k: %d; l: %d\n"
				, pfsm->data.i, pfsm->data.j, pfsm->data.k, pfsm->data.l
			);
	return THIS(noEvent);
}

void UFMN(xlate_e1)(pTEST_FSM pfsm, pTEST_FSM_E1_DATA pe1_data, TEST_FSM_EVENT_ENUM event)
{
	DBG_PRINTF("%s", __func__);

	pfsm->data.pe1_data = pe1_data;

	(void) run_test_fsm_e1_translator(pfsm, event);
}

void UFMN(init_s2_implementor)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	run_test_fsm_s2_implementor(pfsm, THIS(e0));
}

void UFMN(init_s3_implementor)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	run_test_fsm_s3_implementor(pfsm, THIS(e0));
}

