#include <stdio.h>
#include "sub_priv.h"

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;

	return THIS(noEvent);
}

void UFMN(onExitFrom_s1)(pSUB_DATA pdata)
{
	(void) pdata;
	printf("%s\n", __func__);
}

void UFMN(onEntryTo_s2)(pSUB_DATA pdata)
{
	(void) pdata;
	printf("%s\n", __func__);
}

TEST_EVENT __attribute((weak)) UFMN(always)(FSM_TYPE_PTR pfsm)
{
	return sub_pass_shared_event(pfsm, sharing_sub_e1);
}

