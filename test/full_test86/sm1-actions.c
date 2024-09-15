#include "sub_machine1_priv.h"

TOP_LEVEL_EVENT UFMN(a3)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return PARENT(e3);
}

TOP_LEVEL_EVENT UFMN(a2)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e3);
}

TOP_LEVEL_EVENT UFMN(a1)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(e2);
}

TOP_LEVEL_EVENT UFMN(noAction)(pSUB_MACHINE1 pfsm)
{
	(void) pfsm;
   DBG_PRINTF("%s\n", __func__);

   return THIS(noEvent);
}

SUB_MACHINE1_STATE UFMN(checkTransition)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

   return sub_machine1_s3;
}

SUB_MACHINE1_STATE __attribute__((weak)) top_level_sub_machine1_transitionTos2(FSM_TYPE_PTR pfsm,TOP_LEVEL_EVENT e)
{
	(void) e;
	(void) pfsm;

	return sub_machine1_s2;
}

SUB_MACHINE1_STATE __attribute__((weak)) top_level_sub_machine1_transitionTos1(FSM_TYPE_PTR pfsm,TOP_LEVEL_EVENT e)
{
	(void) e;
	(void) pfsm;

	return sub_machine1_s1;
}

SUB_MACHINE1_STATE __attribute__((weak)) UFMN(noTransitionFn)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
	(void) e;
	return pfsm->state;
}

