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

