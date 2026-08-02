#include "test_fsm_priv.h"

ACTION_RETURN_TYPE UFMN(act_on_e1)(FSM_TYPE_PTR pfsm)
{
   DBG_PRINTF("%s", __func__);

	return test_fsm_pass_shared_event(pfsm, sharing_test_fsm_e1);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return THIS(noEvent);
}

TEST_FSM_EVENT_ENUM test_fsm_grab_e1(FSM_DATA_PTR pfsm_data,pTEST_FSM_E1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);

	pfsm_data->e1_count += pdata->i;

	DBG_PRINTF("e1 accumulator: %u", pfsm_data->e1_count);

	return pdata->ext_event ? THIS(e2) : THIS(e1);
}

void UFMN(record)(pTEST_FSM pfsm, TEST_FSM_STATE new_s)
{
    DBG_PRINTF("%s", __func__);
	 (void) pfsm;
	 (void) new_s;
}
