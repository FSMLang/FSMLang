#include "s3_implementor_priv.h"

TEST_FSM_EVENT_ENUM UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return THIS(noEvent);
}

TEST_FSM_EVENT_ENUM UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;

	return THIS(noEvent);
}

