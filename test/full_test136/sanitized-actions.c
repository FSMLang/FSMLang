#include "sanitized_priv.h"

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(a3)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(a4)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

void m1_enter_s1(pM1_DATA pdata)
{
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void m1_enter_s2(pM1_DATA pdata)
{
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void m1_enter_s3(pM1_DATA pdata)
{
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void m1_enter_s4(pM1_DATA pdata)
{
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void m1_grab_e1_data(pM1_DATA pfsm_data,pM1_E1_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void m1_grab_e2_data(pM1_DATA pfsm_data,pM1_E2_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void m1_grab_e3_data(pM1_DATA pfsm_data,pM1_E3_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void m1_grab_e4_data(pM1_DATA pfsm_data,pM1_E4_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}

void m1_grab_e5_data(pM1_DATA pfsm_data,pM1_E5_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;

	DBG_PRINTF("%s", __func__);
}


TR_FN_RETURN_TYPE m1_transitionTos4(FSM_TYPE_PTR pfsm,M1_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}

TR_FN_RETURN_TYPE m1_guard1(pM1 pfsm, M1_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}


TR_FN_RETURN_TYPE m1_guard2(pM1 pfsm, M1_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}

TR_FN_RETURN_TYPE m1_guard3(pM1 pfsm, M1_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}

TR_FN_RETURN_TYPE m1_guard4(pM1 pfsm, M1_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}


TR_FN_RETURN_TYPE m1_transitionTos3(FSM_TYPE_PTR pfsm,M1_EVENT_ENUM e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s3);
	return trfnret;
}

#if defined (FSM_VARIANT_C) || defined (FSM_VARIANT_CC)
TR_FN_RETURN_TYPE UFMN(noTransitionFn)(pM1 pfsm, M1_EVENT_ENUM e)
{
	(void) e;
	DBG_PRINTF("%s\n", __func__);
	return pfsm->state;
}
#endif

int main(void)
{
	printf("Sanitized.\n");

	return 0;
}
