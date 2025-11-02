#include "sub_subSub_priv.h"
#include <stddef.h>

#ifndef DBG_PRINTF
#define DBG_PRINTF(...)
#endif

TEST_EVENT __attribute__((weak)) UFMN(a1)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

TEST_EVENT __attribute__((weak)) UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("weak: %s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

