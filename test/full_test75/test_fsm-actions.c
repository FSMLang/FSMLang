#include "test_fsm_priv.h"

TEST_FSM_STATE UFMN(handle_e1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return STATE(noTransition);
}

TEST_FSM_STATE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return STATE(noTransition);
}

void test_fsm_grab_e1_data(pTEST_FSM_DATA pfsm_data,pTEST_FSM_E1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->e1_count += pdata->i;

	DBG_PRINTF("e1 accumulator: %u", pfsm_data->e1_count);

}

