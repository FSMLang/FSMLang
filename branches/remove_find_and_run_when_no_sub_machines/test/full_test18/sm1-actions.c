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



SUB_MACHINE1_STATE subMachine1_transitionToss2(pSUB_MACHINE1 pfsm1,SUB_MACHINE1_EVENT e)
{
	printf("subMachine1_transitionToss2\n");

	return subMachine1_ss2;
}

SUB_MACHINE1_STATE subMachine1_transitionToss3(pSUB_MACHINE1 pfsm1,SUB_MACHINE1_EVENT e)
{
	printf("subMachine1_transitionToss3\n");

	return subMachine1_ss3;
}


