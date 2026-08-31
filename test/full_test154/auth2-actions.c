#include "auth2_priv.h"

ACTION_RETURN_TYPE UFMN(report_auth2_complete)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	return PARENT(auth2_complete);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

