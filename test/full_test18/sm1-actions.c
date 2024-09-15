#include <stdio.h>

#include "subMachine1_priv.h"

NEW_MACHINE_EVENT UFMN(aa1)(FSM_TYPE_PTR pfsm1)
{
	(void) pfsm1;
	printf("subMachine1_aa1\n");

	return newMachine_noEvent;
}

NEW_MACHINE_EVENT UFMN(aa2)(FSM_TYPE_PTR pfsm1)
{
	(void) pfsm1;
	printf("subMachine1_aa2\n");

	return newMachine_noEvent;
}

NEW_MACHINE_EVENT UFMN(noAction)(FSM_TYPE_PTR pfsm1)
{
	(void) pfsm1;
	printf("subMachine1_noAction\n");

	return newMachine_noEvent;
}



