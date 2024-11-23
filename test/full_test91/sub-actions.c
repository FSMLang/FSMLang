#include <stdio.h>
#include "sub_priv.h"

ACTION_RETURN_TYPE UFMN(always)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;
	printf("%s\n", __func__);

	return THIS(noEvent);
}

ACTION_RETURN_TYPE UFMN(noAction)(FSM_TYPE_PTR pfsm)
{
	(void) pfsm;

	return THIS(noEvent);
}

void UFMN(onExitFrom_s1)(void)
{
	printf("%s\n", __func__);
}

void UFMN(onEntryTo_s2)(void)
{
	printf("%s\n", __func__);
}

