#include "auth1_priv.h"

ACTION_RETURN_TYPE UFMN(send_step1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(send_step2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(report_auth1_complete)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	return PARENT(auth1_complete);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

