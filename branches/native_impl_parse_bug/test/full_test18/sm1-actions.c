#include <stdio.h>

#include "subMachine1.h"

NEW_MACHINE_EVENT subMachine1_aa1(pSUB_MACHINE1 pfsm1)
{
	printf("subMachine1_aa1\n");

	return newMachine_noEvent;
}

NEW_MACHINE_EVENT subMachine1_aa2(pSUB_MACHINE1 pfsm1)
{
	printf("subMachine1_aa2\n");

	return newMachine_noEvent;
}

NEW_MACHINE_EVENT subMachine1_noAction(pSUB_MACHINE1 pfsm1)
{
	printf("subMachine1_noAction\n");

	return newMachine_noEvent;
}




