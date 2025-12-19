#include "parser-test9_priv.h"

void newMachine_a2(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s", __func__);
}

void newMachine_a1(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s", __func__);
}

void newMachine_noAction(pNEW_MACHINE pfsm)
{
   (void) pfsm;

   DBG_PRINTF("%s", __func__);
}

TR_FN_RETURN_TYPE newMachine_transitionFn(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) pfsm;
   (void) e;

	DECLARE_TR_FN_RET_VAR(ret, s1);
	return ret;

}

TR_FN_RETURN_TYPE newMachine_transitionFn1(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
   (void) pfsm;
   (void) e;

	DECLARE_TR_FN_RET_VAR(ret, s1);
	return ret;
}

TR_FN_RETURN_TYPE __attribute__((weak)) newMachine_transitionTos1(FSM_TYPE_PTR pfsm,NEW_MACHINE_EVENT e)
{
	(void) e;
	(void) pfsm;

	DECLARE_TR_FN_RET_VAR(ret, s1);
	return ret;

}

#if defined (FSM_VARIANT_C) || defined (FSM_VARIANT_CC)
NEW_MACHINE_STATE __attribute__((weak)) UFMN(noTransitionFn)(pNEW_MACHINE pfsm, NEW_MACHINE_EVENT e)
{
	(void) e;
	return pfsm->state;
}
#endif

