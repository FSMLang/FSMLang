#include "test_fsm_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return STATE(noTransition);
}

ACTION_RETURN_TYPE UFMN(act_on_e1)(FSM_TYPE_PTR pfsm)
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

ACTION_RETURN_TYPE UFMN(act_on_e2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return STATE(s2);
}

ACTION_RETURN_TYPE UFMN(act_on_e3)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return STATE(s1);
}

void test_fsm_enter_s1(pTEST_FSM_DATA pdata)
{
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void test_fsm_leave_s2(pTEST_FSM_DATA pdata)
{
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}


