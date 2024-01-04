#include "full-test18_priv.h"

NEW_MACHINE_EVENT newMachine_a1(pNEW_MACHINE pfsm)
{
	(void) pfsm;
	printf("newMachine_a1\n");

	return newMachine_noEvent;
}

NEW_MACHINE_EVENT newMachine_a2(pNEW_MACHINE pfsm)
{
	(void) pfsm;
	printf("newMachine_a2n");

	return newMachine_noEvent;
}

NEW_MACHINE_EVENT newMachine_noAction(pNEW_MACHINE pfsm)
{
	(void) pfsm;
	printf("newMachine_noAction");

	return newMachine_noEvent;
}



void newMachine_baz(pNEW_MACHINE pfsm,NEW_MACHINE_STATE state)
{
	(void) pfsm;
	(void) state;
	printf("newMachine_baz");
}


NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm,NEW_MACHINE_EVENT e)
{
	(void) pfsm;
	(void) e;
	printf("newMachine_transitionFn");

	return newMachine_s1;
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm,NEW_MACHINE_EVENT e)
{
	(void) pfsm;
	(void) e;
	printf("newMachine_transitionFn1");

	return newMachine_s2;
}

int main()
{
	run_newMachine(newMachine_e1);
	run_newMachine(newMachine_subMachine1_ee1);

	return 0;
}
