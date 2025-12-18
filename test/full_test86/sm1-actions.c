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

TR_FN_RETURN_TYPE UFMN(checkTransition)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
   (void) pfsm;
   (void) e;

	DECLARE_TR_FN_RET_VAR(ret, s3);
	return ret;
}

TR_FN_RETURN_TYPE __attribute__((weak)) top_level_sub_machine1_transitionTos2(FSM_TYPE_PTR pfsm,TOP_LEVEL_EVENT e)
{
	(void) e;
	(void) pfsm;

	DECLARE_TR_FN_RET_VAR(ret, s2);
	return ret;
}

TR_FN_RETURN_TYPE __attribute__((weak)) top_level_sub_machine1_transitionTos1(FSM_TYPE_PTR pfsm,TOP_LEVEL_EVENT e)
{
	(void) e;
	(void) pfsm;

	DECLARE_TR_FN_RET_VAR(ret, s1);
	return ret;
}

#if defined(FSM_VARIANT_C) || defined(FSM_VARIANT_CC)
SUB_MACHINE1_STATE __attribute__((weak)) UFMN(noTransitionFn)(pSUB_MACHINE1 pfsm, TOP_LEVEL_EVENT e)
{
	(void) e;
	return pfsm->state;
}
#endif

