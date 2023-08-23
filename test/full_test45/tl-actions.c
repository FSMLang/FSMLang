#include <stdio.h>
#include "top_level.h"

int main()
{
	printf("Hello, world.\n");

	return 0;
}


NEW_MACHINE_STATE newMachine_transitionFn(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) pfsm;
	(void) e;
	return THIS(s1);
}

NEW_MACHINE_STATE newMachine_transitionFn1(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) pfsm;
	(void) e;
	return THIS(s1);
}

NEW_MACHINE_STATE newMachine_transitionTos1(pNEW_MACHINE pfsm, ACTION_RETURN_TYPE e)
{
	(void) pfsm;
	(void) e;
	return THIS(s1);
}

void newMachine_baz(pNEW_MACHINE pfsm, NEW_MACHINE_STATE s)
{
	(void) pfsm;
   (void) s;
}
