#include <stdio.h>
#include "test_fsm_priv.h"

#define DBG_PRINTF(...)	printf(__VA_ARGS__); printf("\n");

TEST_EVENT UFMN(a2)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

TEST_EVENT UFMN(a3)(FSM_TYPE_PTR pfsm)
{
	DBG_PRINTF("%s", __func__);
	(void) pfsm;
	return THIS(noEvent);
}

TEST_EVENT UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	return THIS(noEvent);
}

