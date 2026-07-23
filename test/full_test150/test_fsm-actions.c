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

void UFMN(record)(FSM_TYPE_PTR pfsm, TEST_FSM_STATE s)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	(void) s;
}

TEST_FSM_EVENT_ENUM UFMN(grab_e1)(FSM_DATA_PTR pfsm_data, pTEST_FSM_E1_DATA pdata)
{
	DBG_PRINTF("%s", __func__);
	pfsm_data->e1_count += pdata->i;
	DBG_PRINTF("e1 accumulator: %u", pfsm_data->e1_count);
	return (TEST_FSM_EVENT_ENUM) pdata->ext_event;
}
