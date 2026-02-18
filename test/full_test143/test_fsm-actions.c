#include "test_fsm_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
}

ACTION_RETURN_TYPE UFMN(act_on_e1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
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
}

ACTION_RETURN_TYPE UFMN(act_on_e3)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
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

TR_FN_RETURN_TYPE test_fsm_guard1(pTEST_FSM pfsm, TEST_FSM_EVENT_ENUM e)
{
	(void) pfsm;
	(void) e;

	DBG_PRINTF("%s", __func__);

	return STATE(s1);
}

#if defined(FSM_VARIANT_C) || defined(FSM_VARIANT_CC)
TR_FN_RETURN_TYPE UFMN(noTransitionFn)(pTEST_FSM pfsm, TEST_FSM_EVENT_ENUM e)
{
	(void) e;

	return pfsm->state;
}

TR_FN_RETURN_TYPE UFMN(transitionTos2)(pTEST_FSM pfsm, TEST_FSM_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	return STATE(s2);
}
#endif

