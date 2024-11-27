#include "top_level_priv.h"

NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) pfsm;
	(void) e;
	return THIS(s1);
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) pfsm;
	(void) e;
	return THIS(s1);
}

NEW_MACHINE_STATE newMachine_transitionTos1(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) pfsm;
	(void) e;
	return THIS(s1);
}

void newMachine_baz(pNEW_MACHINE pfsm, NEW_MACHINE_STATE s)
{
	(void) pfsm;
   (void) s;
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(doNothing)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

void __attribute__((weak)) newMachine_translate_e1_data(pNEW_MACHINE_DATA pfsm_data,pNEW_MACHINE_E1_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;

	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_translate_e2_data(pNEW_MACHINE_DATA pfsm_data,pNEW_MACHINE_E2_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;

	DBG_PRINTF("weak: %s", __func__);
}

void __attribute__((weak)) newMachine_xlate_e3(pNEW_MACHINE_DATA pfsm_data,pNEW_MACHINE_E3_DATA pdata)
{
	(void) pfsm_data;
	(void) pdata;

	DBG_PRINTF("weak: %s", __func__);
}


NEW_MACHINE_STATE __attribute__((weak)) UFMN(noTransitionFn)(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT_ENUM e)
{
	(void) e;
	return pfsm->state;
}

