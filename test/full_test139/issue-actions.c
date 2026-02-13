#include "issue_priv.h"

ACTION_RETURN_TYPE UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
}

ACTION_RETURN_TYPE UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
}

void UFMN(ef1)(void)
{
	DBG_PRINTF("%s", __func__);
}

TR_FN_RETURN_TYPE UFMN(dp1)(FSM_TYPE_PTR pfsm, FSMISSUE_EVENT e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}


TR_FN_RETURN_TYPE UFMN(dp2)(FSM_TYPE_PTR pfsm, FSMISSUE_EVENT e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}

TR_FN_RETURN_TYPE UFMN(dp3)(FSM_TYPE_PTR pfsm, FSMISSUE_EVENT e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}

TR_FN_RETURN_TYPE UFMN(dp4)(FSM_TYPE_PTR pfsm, FSMISSUE_EVENT e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s4);
	return trfnret;
}

#if defined (FSM_VARIANT_C) || defined (FSM_VARIANT_CC)
TR_FN_RETURN_TYPE UFMN(transitionTos3)(FSM_TYPE_PTR pfsm,FSMISSUE_EVENT e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s3);
	return trfnret;
}

TR_FN_RETURN_TYPE UFMN(transitionTos4)(FSM_TYPE_PTR pfsm,FSMISSUE_EVENT e)
{
	(void) e;
	(void) pfsm;

	DBG_PRINTF("%s", __func__);
	DECLARE_TR_FN_RET_VAR(trfnret, s3);
	return trfnret;
}

TR_FN_RETURN_TYPE UFMN(noTransitionFn)(FSM_TYPE_PTR pfsm, FSMISSUE_EVENT e)
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
