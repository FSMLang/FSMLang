#include "subMachine2_priv.h"
#include <stddef.h>

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(aaa1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

NEW_MACHINE_EVENT_ENUM __attribute__((weak)) UFMN(aaa2)(FSM_TYPE_PTR pfsm)
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

