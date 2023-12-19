#include <stdio.h>

#include "anotherSubMachine1_priv.h"


ACTION_RETURN_TYPE anotherNewMachine_anotherSubMachine1_aa1(FSM_TYPE_PTR pfsm)
{
	printf("%s\n", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

ACTION_RETURN_TYPE anotherNewMachine_anotherSubMachine1_aa2(FSM_TYPE_PTR pfsm)
{
	printf("%s\n", __func__);

	(void) pfsm;

	return THIS(noEvent);
}

